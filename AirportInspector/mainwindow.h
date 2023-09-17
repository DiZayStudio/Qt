#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QTimer>
#include <QCalendarWidget>
#include "QStandardItemModel"
#include "QStandardItem"

#include "secondaryform.h"
#include "db.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sig_RequestToDb(QString request);
    void sig_GetDataAirport(QString request, QDate selectedDate);
    void sig_GetStatistics(QString airport);

private slots:
    void on_pB_load_clicked();
    void on_pB_congestion_clicked();

public slots:

    void ReceiveStatusConnectionToDB(bool status);
    void ReceiveStatusRequestToDB(QSqlError err);
    void Get_airport_name(QMap<QString, QString> airport_name);
    void PrintSchedule(QVector<QString> flight_no, QVector<QDateTime> dateTime, QVector<QString> airport);

private:
    Ui::MainWindow *ui;
    secondaryForm *statForm;
    db* dataBase;
    QMessageBox* msg;

    QTimer *timer;
    QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data ";
};
#endif // MAINWINDOW_H
