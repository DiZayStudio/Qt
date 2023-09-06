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

public slots:
    void on_pB_close_clicked();
    void PrintStat(QVector<int> statYear, QVector<QString> statYearTime, QVector<int> statDay, QVector<QString> statDayTime);

private:
    Ui::secondaryForm *ui;

    QBarSet *barSet;
    QBarSeries *series;
    QChart *chart;
    QChartView *chartView;

    QLineSeries *seriesDay;
    QChart *chartDay;
    QChartView *chartViewDay;
public:
    QVector<int> statYear;
    QVector<QString> statYearTime;
    QVector<int> statDay;
    QVector<QString> statDayTime;

signals:
    sig_GetStatistics(QString airport);
    sig_CurrentIndexChanged(int index);

};

#endif // SECONDARYFORM_H
