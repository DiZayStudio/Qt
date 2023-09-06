#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    void Start();

signals:
    sig_Start();
};

#endif // STOPWATCH_H
