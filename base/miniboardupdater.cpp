#include "miniboardupdater.h"

MiniBoardUpdater::MiniBoardUpdater(QObject *parent)
    : QThread(parent)
{
    runTime.start();
}

void MiniBoardUpdater::run()
{
    qDebug() << "Update timer on";
    updateAll();
    qDebug() << "Update timer off";
}

void MiniBoardUpdater::updateAll()
{
    while(m_run){
        //if runtime has gone 1000 ms more
        if ((runTime.elapsed() % 1000) == 0){
            //qDebug() << "updating UI";
            //TODO: pull all things UI needs to display from robot


        }
    }
}

void MiniBoardUpdater::stop()
{
    m_run = false;
}

int MiniBoardUpdater::timeRunning()
{
    return runTime.elapsed();
}

