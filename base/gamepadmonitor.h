#ifndef GAMEPADMONITOR_H
#define GAMEPADMONITOR_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class GamepadMonitor : public QObject
{
    Q_OBJECT
public:
    explicit GamepadMonitor(QObject *parent = 0);
    ~GamepadMonitor();

signals:

public slots:

private:
    QGamepad *m_gamepad;
};

#endif // GAMEPADMONITOR_H
