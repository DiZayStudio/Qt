#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */
    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDBT, this, &MainWindow::ScreenDataFromDBT);
     connect(dataBase, &DataBase::sig_SendDataFromDBQ, this, &MainWindow::ScreenDataFromDBQ);
    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);
}

MainWindow::~MainWindow()
{
    dataBase->DisconnectFromDataBase(DB_NAME);
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */
void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    ui->tableView->setModel(0);
    ui->tableView->reset();
    ui->tableView->update();
     auto req = [&]{dataBase->RequestToDB(ui->cb_category->currentIndex());};
    QtConcurrent::run(req);
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDBT(QSqlTableModel *model)
{
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Название фильма"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание фильма"));
    ui->tableView->setModel(model);

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();

    for( int i = 0; i<model->columnCount(); ++i ) {
        if ( i > 2 ) {
            ui->tableView->hideColumn(i);
        }
    }
    ui->tableView->update();
}

void MainWindow::ScreenDataFromDBQ(QSqlQueryModel *model)
{
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Название фильма"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Описание фильма"));
    ui->tableView->setModel(model);
 //   ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();

    for( int i = 0; i<model->columnCount(); ++i ) {
        if ( i > 2 ) {
            ui->tableView->hideColumn(i);
        }
    }
    ui->tableView->update();
}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }
}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{
    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else{
        dataBase->RequestToDB(ui->cb_category->currentIndex());
    }
}


void MainWindow::on_pb_clear_clicked()
{
    // очистка tableView
    ui->tableView->setModel(0);
    ui->tableView->reset();
    ui->tableView->update();
}

