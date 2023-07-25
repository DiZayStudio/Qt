#include "signals.h"

Signals::Signals(QObject *parent) : QObject{parent}
{

}

Signals::~Signals(){
}

void Signals::SendStartStop()
{
   emit sig_SendStart_Stop();
}
