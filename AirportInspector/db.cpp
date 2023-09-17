#include "db.h"

db::db(QObject *parent)
    : QObject{parent}
{
    status_connect = false;
    dataBase = new QSqlDatabase();
    query = new QSqlQuery();

    data.resize(5);
    data[hostName] = "981757-ca08998.tmweb.ru";
    data[dbName] = "demo";
    data[login] = "netology_usr_cpp";
    data[pass] = "CppNeto3";
    data[port] = "5432";
}

db::~db()
{
    delete dataBase;
}


void db::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

    DBConection();
}


// Метод подключается к БД
void db::ConnectToDataBase()
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool status;
    status = dataBase->open();

    emit sig_SendStatusConnection(status);
}

void db::DBConection()
{
    if(!status_connect){
       auto conn = [&]{ConnectToDataBase();};
       auto tmp = QtConcurrent::run(conn);
    }
    else{
        DisconnectFromDataBase(DB_NAME);
    }
}

// Метод производит отключение от БД
void db::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}

void db::GetSchedule(QString request, QDate selectedDate)
{
    *query = QSqlQuery(*dataBase);

    QSqlError err;
    if(query->exec(request) == false){
        err = query->lastError();
    } else {

    flight_no.clear();
    time.clear();
    airport.clear();

        while(query->next()){
            QDateTime dt = QDateTime::fromString(query->value(1).toString(), "yyyy-MM-ddTHH:mm:ss.zzz");
           if(dt.date() == selectedDate){
           flight_no.push_back(query->value(0).toString());
           time.push_back(dt);
           airport.push_back(query->value(2).toString());
           }
        }
    emit sig_PrintSchedule(flight_no, time, airport);
    }
}

void db::GetStatistics(QString airportCode)
{
    *query = QSqlQuery(*dataBase);
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and (departure_airport='" + airportCode + "' or arrival_airport='" + airportCode + "') group by \"Month\"";

    QSqlError err;
    if(query->exec(request) == false){
        err = query->lastError();
    }

    statYear.clear();
    statYearDate.clear();
    while(query->next()){
        statYear.push_back(query->value(0).toInt());
        statYearDate.push_back(query->value(1).toString());
    }

    request = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
            "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and (departure_airport='" + airportCode +"' or arrival_airport='" + airportCode + "') GROUP BY \"Day\"";

    if(query->exec(request) == false){
        err = query->lastError();
    }
    statDay.clear();
    statDayDate.clear();
    while(query->next()){
        statDay.push_back(query->value(0).toInt());
        statDayDate.push_back(query->value(1).toString());
    }

    emit sig_PrintStat(statYear, statYearDate, statDay, statDayDate);
}

// Метод формирует запрос к БД.
void db::RequestToDB(QString request)
{
    *query = QSqlQuery(*dataBase);

    QSqlError err;
    if(query->exec(request) == false){
        err = query->lastError();
    }

    while(query->next()){
        airport_name[query->value(0).toString()] = query->value(1).toString();
    }
    emit sig_NameAirport(airport_name);
}

void db::ReadAnswer(int direction)
{
    if (direction == 0) {

        flight_no.clear();

        while(query->next()){
           flight_no.push_back(query->value(0).toString());
        }
    }
}

// Метод возвращает последнюю ошибку БД
QSqlError db::GetLastError()
{
    return dataBase->lastError();
}
