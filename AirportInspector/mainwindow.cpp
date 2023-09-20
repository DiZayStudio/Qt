#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    statForm = new secondaryForm();
    statForm->hide();

    //Добавим БД используя стандартный драйвер PSQL и зададим имя.
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    timer = new QTimer(this);

    connect(dataBase, &db::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &db::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);
    connect(timer, &QTimer::timeout, dataBase, &db::DBConection);
    connect(dataBase, &db::sig_NameAirport, this, &MainWindow::Get_airport_name);
    connect(this, &MainWindow::sig_GetDataAirport, dataBase, &db::GetSchedule);
    connect(dataBase, &db::sig_PrintSchedule, this, &MainWindow::PrintSchedule);

    connect(statForm, &secondaryForm::sig_GetStatistics, dataBase, &db::GetStatistics);
    connect(this, &MainWindow::sig_GetStatistics, dataBase, &db::GetStatistics);
    connect(dataBase, &db::sig_PrintStat, statForm, &secondaryForm::PrintStat);
}

MainWindow::~MainWindow()
{
    dataBase->DisconnectFromDataBase(DB_NAME);
    delete ui;
    delete statForm;
}

void MainWindow::on_pB_load_clicked()
{
    QDate selectedDate = ui->dateEdit->date();
    QString airport_name = ui->cB_airport_name->currentText();
    int direction = ui->cB_direction->currentIndex();

    emit  sig_GetDataAirport(direction, selectedDate, airport_name);
}

void MainWindow::on_pB_congestion_clicked()
{
    statForm->setWindowTitle("Загруженность аэропорта " + ui->cB_airport_name->currentText() );
    statForm->setWindowModality(Qt::ApplicationModal);
    statForm->airportName = dataBase->airport_name[ui->cB_airport_name->currentText()];
    emit sig_GetStatistics(dataBase->airport_name[ui->cB_airport_name->currentText()]);
    statForm->show();
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

void MainWindow::PrintSchedule(QVector<QString> flight_no, QVector<QDateTime> dateTime, QVector<QString> airport)
{
    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    QStandardItem *item;

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
        item = new QStandardItem(flight_no[i]);
        model->setItem(j, 0, item);
        item = new QStandardItem((dateTime[i].time()).toString());
        model->setItem(j, 1, item);
        item = new QStandardItem(airport[i]);
        model->setItem(j, 2, item);
        j++;
    }
    //Выводим данные
    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    ui->pB_congestion->setEnabled(true);
}
