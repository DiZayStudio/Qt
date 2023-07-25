#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void StartStop();

    void addTime();
    QString getTimeStr();
    void clear();
    QString getCircleTimeStr();
    QString TimeToString(int m, int s, int ms);
    int getNumCircle();

signals:
    void sig_Start_Stop();

private:
    int _m = 0;
    int _s = 0;
    int _ms = 0;
    int _Start_m = 0;
    int _Start_s = 0;
    int _Start_ms = 0;
    int _num_circle = 0;
};

#endif // STOPWATCH_H
