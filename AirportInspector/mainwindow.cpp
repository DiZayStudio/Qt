#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    status_connect = false;
    // Исходное состояние виджетов
    this->setWindowTitle("Инспектор аэропортов");

    ui->pB_load->setStyleSheet("padding: 10px;");
    ui->pB_congestion->setStyleSheet("padding: 10px;");

    ui->dateEdit->setDate(QDate(2016, 8, 15));
    ui->dateEdit->setMaximumDate(QDate(2017, 9, 14));
    ui->dateEdit->setMinimumDate(QDate(2016, 8, 15));
    ui->dateEdit->setCalendarPopup(true);

    ui->cB_direction->addItem("Прилёт");
    ui->cB_direction->addItem("Вылет");
    ui->pB_congestion->setEnabled(false);
    ui->pB_load->setEnabled(false);

    ui->statusbar->showMessage("Подключаемся к БД");
    dataBase = new db(this);
    msg = new QMessageBox(this);

    form = new secondaryForm();
    form->hide();

    //Добавим БД используя стандартный драйвер PSQL и зададим имя.
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);


    stw = new Stopwatch(this);
    timer = new QTimer(this);

    DBConection();

    connect(dataBase, &db::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &db::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);
    connect(timer, &QTimer::timeout, this, &MainWindow::DBConection);
    connect(dataBase, &db::sig_NameAirport, this, &MainWindow::Get_airport_name);
    connect(this, &MainWindow::sig_GetDataAirport, dataBase, &db::GetSchedule);
    connect(dataBase, &db::sig_PrintSchedule, this, &MainWindow::PrintSchedule);

    connect(form, secondaryForm::sig_GetStatistics, dataBase, &db::GetStatistics);
    connect(this, &MainWindow::sig_GetStatistics, dataBase, &db::GetStatistics);
    connect(dataBase, &db::sig_PrintStat, form, secondaryForm::PrintStat);
}

MainWindow::~MainWindow()
{
    dataBase->DisconnectFromDataBase(DB_NAME);
    delete ui;
    delete form;
}


void MainWindow::on_pB_load_clicked()
{
    QString airport_name = ui->cB_airport_name->currentText();
    int direction = ui->cB_direction->currentIndex();
    QString request;
    if (direction == 0) {
        request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                  "WHERE f.arrival_airport  = '" + dataBase->airport_name[airport_name] + "'";
        } else {
        request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                  "WHERE f.departure_airport  = '" + dataBase->airport_name[airport_name] + "'";
        }
    emit  sig_GetDataAirport(request);
}

void MainWindow::on_pB_congestion_clicked()
{
    form->setWindowTitle("Загруженность аэропорта " + ui->cB_airport_name->currentText() );
    form->setWindowModality(Qt::ApplicationModal);
    form->airportName = dataBase->airport_name[ui->cB_airport_name->currentText()];
    emit sig_GetStatistics(dataBase->airport_name[ui->cB_airport_name->currentText()]);
    form->show();
}


void MainWindow::DBConection()
{
    if(!status_connect){

       auto conn = [&]{dataBase->ConnectToDataBase();};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
    }
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->statusbar->showMessage("БД: подключено");
        ui->statusbar->setStyleSheet("color:green");
        ui->pB_load->setEnabled(true);

        dataBase->RequestToDB(request);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->statusbar->showMessage("БД: нет соединения");
        ui->statusbar->setStyleSheet("color:red");
        ui->pB_congestion->setEnabled(false);
        ui->pB_load->setEnabled(false);

        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        msg->exec();

        timer->start(5000);
    }
}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{
    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else{
        dataBase->ReadAnswer(ui->cB_direction->currentIndex());
    }
}

void MainWindow::Get_airport_name(QMap<QString, QString> airport_name)
{
    QMap<QString, QString>::iterator it = airport_name.begin();
    for(; it != airport_name.end();++it){
    ui->cB_airport_name->addItem(it.key());
    }
}

void MainWindow::PrintSchedule(QVector<QString> flight_no, QVector<QString> dataTime, QVector<QString> airport)
{
    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    QStandardItem *item;

    QDate select_date = ui->dateEdit->date();

    //Заголовки столбцов
    QStringList horizontalHeader;
    if (ui->cB_direction->currentIndex() == 0){
        horizontalHeader.append("Номер рейса");
        horizontalHeader.append("Время вылета");
        horizontalHeader.append("Аэропорт отправления");
    } else {
        horizontalHeader.append("Номер рейса");
        horizontalHeader.append("Время вылета");
        horizontalHeader.append("Аэропорт назначения");
    }
    model->setHorizontalHeaderLabels(horizontalHeader);
    int j = 0;
    for(int i = 0; i< flight_no.size();++i){

        QDateTime dt = QDateTime::fromString(dataTime[i], "yyyy-MM-ddTHH:mm:ss.zzz");

        if (dt.date() == select_date){
        item = new QStandardItem(flight_no[i]);
        model->setItem(j, 0, item);
        item = new QStandardItem((dt.time()).toString());
        model->setItem(j, 1, item);
        item = new QStandardItem(airport[i]);
        model->setItem(j, 2, item);
        j++;
        }
    }
    //Выводим данные
    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    ui->pB_congestion->setEnabled(true);
}


