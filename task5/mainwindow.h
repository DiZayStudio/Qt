#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_pB_start_clicked();
    void on_pB_clear_clicked();
    void updateTime();
private slots:
    void on_pB_circle_clicked();

private:
    Ui::MainWindow *ui;
    Stopwatch *stw;

    bool Works = false;
    QTimer *timer;
    QTime *currentTime;
};
#endif // MAINWINDOW_H
