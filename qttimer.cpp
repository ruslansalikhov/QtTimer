#include "qttimer.h"


QtTimer *QtTimer::instance()
{
    static QtTimer *timer = new QtTimer;
    return timer;
}

void QtTimer::start(int msec)
{
    m_timer.start(msec);
}

void QtTimer::stop()
{
    m_timer.stop();
}

void QtTimer::onTimeout()
{
    emit timeout();
}

QtTimer::QtTimer(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &QtTimer::onTimeout);
}

QtTimer::~QtTimer()
{
}
