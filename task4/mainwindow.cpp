#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->rB_celsius->setText("Градусы");
    ui->rB_fahrenheit->setText("Фаренгейт");

    ui->cB_region->addItem("Московская обл.");
    ui->cB_region->addItem("Ярославская обл.");
    ui->cB_region->addItem("Воронежская обл.");
    ui->cB_region->addItem("Тверская обл.");
    ui->cB_region->addItem("Курская обл.");
    ui->cB_region->addItem("Тамбовская обл.");

    ui->pB_Load->setText("Нажать");
    ui->pB_Load->setCheckable(true);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(10);
    ui->progressBar->setValue(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pB_Load_clicked()
{
    if(ui->progressBar->value() < 10){
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    } else {
        ui->progressBar->setValue(0);
    }
}

