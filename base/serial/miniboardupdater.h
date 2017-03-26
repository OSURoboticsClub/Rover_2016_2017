#ifndef MINIBOARDUPDATER_H
#define MINIBOARDUPDATER_H

#include <QObject>
#include <QThread>
#include <QTime>

#include "serial/serialhandler.h"

class MiniBoardUpdater : public QThread
{
    Q_OBJECT
public:
    MiniBoardUpdater(QObject *parent = 0);
    int timeRunning();
private:
    void run();
    void eventLoop();
    bool m_run = true;
    QTime runTime;
signals:
    void update();
    void changeButtonColor(QString,bool);

public slots:
    void stop();
};

#endif // MINIBOARDUPDATER_H
