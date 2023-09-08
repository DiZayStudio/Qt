#include "secondaryform.h"
#include "ui_secondaryform.h"
#include "db.h"

secondaryForm::secondaryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::secondaryForm)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Статистика за год");
    ui->tabWidget->setTabText(1, "Статистика за месяц");

    for(int i=1; i<=12; ++i)
    ui->comboBox->addItem(QLocale::system().monthName(i, QLocale::ShortFormat));


 //   connect(ui->comboBox, currentIndexChanged,this, secondaryForm.PrintStatDay());
}

secondaryForm::~secondaryForm()
{
    delete ui;

}

void secondaryForm::PrintStatYear()
{
        series = new QBarSeries;
        barSet = new QBarSet("За год");
        chart = new QChart();
        chartView = new QChartView(chart);

        // отображение статистики за год
        ui->lay_Charts->addWidget(chartView);

        for(int i = 0; i < statYear.size(); i++){
            barSet->append(statYear.at(i));
        }

        series->append(barSet);

        QStringList categories;
        for(int i=0;i<statYearTime.size();++i){
            QDateTime date = QDateTime::fromString(statYearTime.at(i),"yyyy-MM-ddThh:mm:ss.zzz");
        categories << date.toString("MMM");
        }

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
    //    axisY->setRange(0,300);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->show();
}

void secondaryForm::PrintStatDay()
{
    seriesDay = new QLineSeries;
    chartDay = new QChart();
    chartViewDay = new QChartView(chartDay);

    seriesDay->clear();
    // отображение статистики по месяцам
    int month = ui->comboBox->currentIndex()+1;

        ui->gridLayout->addWidget(chartViewDay);

        for(int i = 0; i < statDay.size(); i++){

            int date = (statDayTime.at(i).mid(5,2)).toInt();
            if (month == date){
               QString tmp = statDayTime.at(i).mid(8,2);
            seriesDay->append((statDayTime.at(i).mid(8,2)).toInt(), statDay.at(i));
            }
        }
        chartDay->legend()->setVisible(false);
        chartDay->legend()->hide();
        chartDay->addSeries(seriesDay);
        chartDay->createDefaultAxes();

     //  createDefaultChartView(chart);
        chartViewDay->setRenderHint(QPainter::Antialiasing);
        chartViewDay->show();
}

void secondaryForm::on_pB_close_clicked()
{
    chartView->chart()->deleteLater();

//    delete chartView;
//    delete chartViewDay;
//    delete chart;
//    delete chartDay;
//    delete series;
//    delete seriesDay;
//    delete barSet;

    this->close();
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
    //this->PrintStatDay();
    emit sig_CurrentIndexChanged(index);

}

