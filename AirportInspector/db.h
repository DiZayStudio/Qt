#ifndef DB_H
#define DB_H

#include <QObject>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlQuery>
#include <QtConcurrent>
#include <QSqlError>
#include <QDebug>


#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

class db : public QObject
{
    Q_OBJECT
public:
    explicit db(QObject *parent = nullptr);
    ~db();
    void AddDataBase(QString driver, QString nameDB = "");
    void RequestToDB(QString request);
    void ReadAnswer(int direction);
    QSqlError GetLastError(void);
    void ConnectToDataBase();

    void DisconnectFromDataBase(QString nameDb = "");


signals:
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError err);
   void sig_NameAirport(QMap<QString, QString> airport_name);
   void sig_ReceiveStatusRequest(QSqlError err);
   void sig_PrintSchedule(QVector<QString> flight_no, QVector<QDateTime> time, QVector<QString> airport);
   void sig_PrintStat(QVector<int> statYear, QVector<QString> statYearTime, QVector<int> statDay, QVector<QString> statDayTime);

private:
    QSqlDatabase* dataBase;
    QSqlQuery* query;
    QVector<QString> data;

    QVector<QString> flight_no;
    QVector<QDateTime> time;
    QVector<QString> airport;
    bool status_connect = false;

public:
    QMap<QString, QString> airport_name;
    QVector<int> statYear;
    QVector<QString> statYearDate;
    QVector<int> statDay;
    QVector<QString> statDayDate;

public slots:
    void DBConection();
    void GetSchedule(int direction, QDate selectedDate, QString airportName);
    void GetStatistics(QString airport);

};

#endif // DB_H
