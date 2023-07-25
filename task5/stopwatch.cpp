#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{

}

void Stopwatch::StartStop()
{
    emit sig_Start_Stop();
}

void Stopwatch::addTime()
{
    _ms++;
    if(_ms >=10){
        _ms = 0;
        _s++;
    }
    if(_s >=60){
        _s = 0;
        _m++;
    }
}

QString Stopwatch::getTimeStr()
{
    return TimeToString(_m, _s, _ms);
}

void Stopwatch::clear()
{
    _m = 0;
    _s = 0;
    _ms= 0;
    _num_circle = 0;
}

QString Stopwatch::getCircleTimeStr()
{
    _num_circle++;
    if(_num_circle == 1){
        _Start_m = _m;
        _Start_s = _s;
        _Start_ms= _ms;
        return TimeToString(_m,_s,_ms);
    } else{
        int dif_m, dif_s, dif_ms;
        dif_m = _m - _Start_m;
        dif_s = _s - _Start_s;
        if (dif_s < 0){
            dif_s += 60;
            dif_m--;
        }
        dif_ms = _ms - _Start_ms;
        if (dif_ms < 0){
            dif_ms += 10;
            dif_s--;
        }

        QString tmp = TimeToString(dif_m, dif_s, dif_ms);
        _Start_m = _m;
        _Start_s = _s;
        _Start_ms= _ms;
        return tmp;
    }
}

QString Stopwatch::TimeToString(int m, int s, int ms)
{
    QString min, sec;
    if(s < 10){
        sec = "0" + QString::number(s);
    }else{
        sec = QString::number(s);
    }
    if(m < 10){
        min = "0" + QString::number(m);
    }else{
        min = QString::number(m);
    }
    return min + ":" + sec + "," + QString::number(ms);
}

int Stopwatch::getNumCircle()
{
    return _num_circle;
}
