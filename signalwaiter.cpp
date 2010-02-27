#include <QTimer>
#include <QEventLoop>
#include "signalwaiter.h"

SignalWaiter::SignalWaiter(const QObject* sender, const char* signal) :
    m_timer(new QTimer(this)),
    m_eventLoop(new QEventLoop(this)),
    QObject(0)
{
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), m_eventLoop, SLOT(quit()));
    connect(sender, signal, m_eventLoop, SLOT(quit()));
}

bool SignalWaiter::wait(int msec)
{
    m_timer->start(msec);
    m_eventLoop->exec();

    if (m_timer->isActive()) {
        m_timer->stop();
        return true;
    } else {
        return false;
    }
}
