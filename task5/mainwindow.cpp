#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lb_time->setText("00:00,0");

    stw = new Stopwatch(this);

    ui->pB_circle->setEnabled(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    stw->addTime();
    ui->lb_time->setText(stw->getTimeStr());
}

void MainWindow::on_pB_start_clicked()
{
    emit stw->sig_Start_Stop();
    if(Works == false){

        ui->pB_start->setText("Стоп");
        Works = true;
        ui->pB_circle->setEnabled(true);
        timer->start(100);
    } else {
        ui->pB_start->setText("Старт");
        Works = false;
        ui->pB_circle->setEnabled(false);
        timer->stop();
    }
}


void MainWindow::on_pB_clear_clicked()
{
    ui->textBrowser->clear();
    stw->clear();
    ui->lb_time->setText(stw->getTimeStr());
}


void MainWindow::on_pB_circle_clicked()
{
    ui->textBrowser->append("Круг " + QString::number(stw->getNumCircle()) + ", время: " + stw->getCircleTimeStr());
}

