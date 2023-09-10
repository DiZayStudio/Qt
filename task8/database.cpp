#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    queryModel = new QSqlQueryModel();
    tableModel = new QSqlTableModel();
}

DataBase::~DataBase()
{
    delete dataBase;
    delete tableModel;
    delete queryModel;
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
    status = dataBase->open();
    emit sig_SendStatusConnection(status);
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
void DataBase::RequestToDB(int requestType)
{
    switch (requestType) {
    case requestAllFilms:
        tableModel = new QSqlTableModel(this, *dataBase);
        tableModel->setTable("film");
        tableModel->select();
    emit sig_SendDataFromDBT(tableModel);
    break;
    case requestComedy:
        queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(request1, *dataBase);
    emit sig_SendDataFromDBQ(queryModel);
        break;
    case requestHorrors:
           queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(request2, *dataBase);
    emit sig_SendDataFromDBQ(queryModel);
        break;
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
