#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
enum requestType{
    requestAllFilms = 0,
    requestComedy   = 1,
    requestHorrors  = 2
};


class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(int requestType);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);


signals:
   void sig_SendDataFromDBT(QSqlTableModel *model);
   void sig_SendDataFromDBQ(QSqlQueryModel *model);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError err);

private:
    QSqlDatabase* dataBase;
    QSqlQueryModel* queryModel;
    QSqlTableModel* tableModel;

    QString request = "SELECT title, release_year, c.name  FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id  = fc.category_id";

    QString request1= "SELECT title, description FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id = fc.category_id "
                      "WHERE c.name = 'Comedy'";

    QString request2= "SELECT title, description FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id = fc.category_id "
                      "WHERE c.name = 'Horror'";
};

#endif // DATABASE_H
