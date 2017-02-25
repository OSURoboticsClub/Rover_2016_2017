#include "miniboardupdater.h"

MiniBoardUpdater::MiniBoardUpdater(QObject *parent)
    : QThread(parent)
{
    runTime.start();
    connect(this, SIGNAL(updateAll()), Handler, SLOT(readBatteryVoltage()));
}

void MiniBoardUpdater::run()
{

    qDebug() << "Update timer on";
    eventLoop();
    qDebug() << "Update timer off";
    m_run = true;
}

void MiniBoardUpdater::eventLoop()
{
    while(m_run){
        //if runtime has gone 1000 ms more
        //if ((runTime.elapsed() % 5000) == 0){
            //qDebug() << "updating UI";
            //TODO: pull all things UI needs to display from robot
            //Handler->readPause();
            msleep(1500);
            emit updateAll();
            //Handler->readBatteryVoltage();
//            Handler->readDriveMotorPower();
//            Handler->readSwerveDriveState();
//            Handler->readArmMotors();
//            Handler->readPotentiometers();
//            Handler->readSelectCamera();
//            Handler->readCallsign();
//            //Handler->readGpsPosition();
//            //Handler->readGpsTrack();
//            Handler->readMagnetometer();
//            //Handler->readAccelerometer();
//            Handler->readGyroscope();
//            //Handler->readCompassHeading();
//            Handler->readGpioDirection();
//            //Handler->readGpioOutValue();
//            //Handler->readGpioReadState();
//            Handler->readDebuggingInfo();
//            Handler->readBuildInfo();

        //}
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

