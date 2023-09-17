#include "secondaryform.h"
#include "ui_secondaryform.h"
//#include "db.h"

secondaryForm::secondaryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::secondaryForm)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Статистика за год");
    ui->tabWidget->setTabText(1, "Статистика за месяц");

    for(int i=1; i<=12; ++i)
    ui->comboBox->addItem(QLocale::system().monthName(i, QLocale::ShortFormat));

    connect(ui->comboBox, &QComboBox::currentIndexChanged,this, &secondaryForm::PrintStatDay);

    series = new QBarSeries;
    barSet = new QBarSet("За год");
    chart = new QChart();
    chartView = new QChartView(chart);
    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis;
    axisXDay = new QValueAxis;
    axisYDay = new QValueAxis;
    seriesDay = new QLineSeries;
    chartDay = new QChart();
    chartViewDay = new QChartView(chartDay);
}

secondaryForm::~secondaryForm()
{
    delete series;
    delete barSet;
    delete chart;
    delete chartView;
    delete seriesDay;
    delete chartDay;
    delete chartViewDay;
    delete axisX;
    delete axisY;

    delete ui;
}

void secondaryForm::PrintStatYear()
{
    //очистка графиков
    series->clear();
    if(chart->series().isEmpty() == false){
            chart->removeSeries(series);
            chart->removeAxis(axisX);
            chart->removeAxis(axisY);

    }

    int max = 0;
    // отображение статистики за год
    for(int i = 0; i < statYear.size(); i++){
        barSet->append(statYear.at(i));
        if(statYear.at(i) > max){
            max = statYear.at(i);
        }
    }

    series->append(barSet);

    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList months;
    for(int i=0;i<statYearTime.size();++i){
        QDateTime date = QDateTime::fromString(statYearTime.at(i),"yyyy-MM-ddThh:mm:ss.zzz");
    months << date.toString("MMM");
    }

    axisX->setTitleText("месяцы");
    axisX->append(months);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisY->setTitleText("количество");
    axisY->setLabelFormat("%.0f");
    axisY->setRange(0,max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->lay_Charts->addWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}

void secondaryForm::PrintStatDay()
{
    //очистка графиков
    seriesDay->clear();
    if(chartDay->series().isEmpty() == false){
            chartDay->removeSeries(seriesDay);
            chartDay->removeAxis(axisXDay);
            chartDay->removeAxis(axisYDay);
    }

    // отображение статистики по месяцам
    int month = ui->comboBox->currentIndex()+1;
    int max = 0;
        for(int i = 0; i < statDay.size(); i++){
            if (statDay.at(i)>max){
                max = statDay.at(i);
            }

            int date = (statDayTime.at(i).mid(5,2)).toInt();
            if (month == date){
               QString tmp = statDayTime.at(i).mid(8,2);
            seriesDay->append((statDayTime.at(i).mid(8,2)).toInt(), statDay.at(i));
            }
        }
        chartDay->legend()->hide();
        chartDay->addSeries(seriesDay);

        axisXDay->setTitleText("дни");
        axisXDay->setLabelFormat("%.0f");

        axisYDay->setTitleText("количество");
        axisYDay->setLabelFormat("%.0f");

        chartDay->addAxis(axisXDay, Qt::AlignBottom);
        chartDay->addAxis(axisYDay, Qt::AlignLeft);

        seriesDay->attachAxis(axisXDay);
        seriesDay->attachAxis(axisYDay);

        axisYDay->setRange(0, max);

        ui->gridLayout->addWidget(chartViewDay);
        chartViewDay->setRenderHint(QPainter::Antialiasing);
        chartViewDay->show();
}

void secondaryForm::on_pB_close_clicked()
{
    this->hide();
}

void secondaryForm::PrintStat(QVector<int> statYear, QVector<QString> statYearTime, QVector<int> statDay, QVector<QString> statDayTime)
{
   this->statYear = statYear;
   this->statYearTime = statYearTime;
   this->statDay = statDay;
   this->statDayTime = statDayTime;

   // отображение статистики за год
   PrintStatYear();
   // отображение статистики по месяцам
   PrintStatDay();
}

void secondaryForm::on_comboBox_currentIndexChanged(int index)
{
    emit sig_CurrentIndexChanged(index);
}

