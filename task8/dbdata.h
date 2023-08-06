#ifndef DBDATA_H
#define DBDATA_H

#include <QDialog>

namespace Ui {
class DbData;
}

class DbData : public QDialog
{
    Q_OBJECT

public:
    explicit DbData(QWidget *parent = nullptr);
    ~DbData();


 signals:
    void sig_sendData(QVector<QString> dbData);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::DbData *ui;
    QVector<QString> data;

};

#endif // DBDATA_H
