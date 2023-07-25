#ifndef SIGNALS_H
#define SIGNALS_H

#include <QObject>
#include <QDebug>

class Signals : public QObject
{
    Q_OBJECT
public:
    explicit Signals(QObject *parent = nullptr);
    ~Signals();
    void SendStartStop();

signals:
    void sig_SendStart_Stop();
};

#endif // SIGNALS_H
