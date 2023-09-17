#ifndef SECONDARYFORM_H
#define SECONDARYFORM_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QDate>

namespace Ui {
class secondaryForm;
}

class secondaryForm : public QWidget
{
    Q_OBJECT

public:
    explicit secondaryForm(QWidget *parent = nullptr);
    ~secondaryForm();

    QString airportName;
    void PrintStatYear();
    void PrintStatDay();
    void on_comboBox_currentIndexChanged(int index);

    QVector<int> statYear;
    QVector<QString> statYearTime;
    QVector<int> statDay;
    QVector<QString> statDayTime;

    QLineSeries *seriesDay;
    QChart *chartDay;
    QChartView *chartViewDay;

    QBarSeries *series;
    QBarSet *barSet;
    QChart *chart;
    QChartView *chartView;

    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

    QValueAxis *axisXDay;
    QValueAxis *axisYDay;
public slots:
    void on_pB_close_clicked();
    void PrintStat(QVector<int> statYear, QVector<QString> statYearTime, QVector<int> statDay, QVector<QString> statDayTime);

private:
    Ui::secondaryForm *ui;  

signals:
    void sig_GetStatistics(QString airport);
    void sig_CurrentIndexChanged(int index);

};

#endif // SECONDARYFORM_H
