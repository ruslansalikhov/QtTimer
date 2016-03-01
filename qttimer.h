#ifndef QTTIMER_H
#define QTTIMER_H

#include "qttimer_global.h"

#include <QTimer>

class QTTIMERSHARED_EXPORT QtTimer : public QObject
{
    Q_OBJECT

public:
    static QtTimer* instance();

    void start(int msec);
    void stop();

signals:
    void timeout();

private slots:
    void onTimeout();

private:
    /*! singleton object */
    QtTimer(QObject* parent=nullptr);
    ~QtTimer();
    Q_DISABLE_COPY(QtTimer)

    void initPlatformData();

    QTimer m_timer;

};

#endif // QTTIMER_H
