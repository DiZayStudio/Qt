#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    model = new QSqlQueryModel();
}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    ///Тут должен быть код ДЗ
    bool status;
    status = dataBase->open( );

    if (status) {
        model = new QSqlTableModel(this, *dataBase);
     //   q_model = new QSqlQueryModel(this, *dataBase);

        emit sig_SendStatusConnection(status);
    };
}

/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{
    *simpleQuery = QSqlQuery(*dataBase);

    QSqlError err;
    if(simpleQuery->exec(request) == false){
        err = simpleQuery->lastError();
    }

    emit sig_SendStatusRequest(err);
}

void DataBase::ReadAnswerFromDB(int requestType)
{
    switch (requestType) {

    case requestAllFilms:
    case requestComedy:
    case requestHorrors:
    {
        model->setTable("film");
        model->select();
        emit sig_SendDataFromDB(model, requestAllFilms);
        break;
    }

    default:
        break;
    }
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
