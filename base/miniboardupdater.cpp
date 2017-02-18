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
            //Handler->readPause();
            Handler->readBatteryVoltage();
            Handler->readDriveMotorPower();
            Handler->readSwerveDriveState();
            Handler->readArmMotors();
            //Handler->readPotentiometers();
            Handler->readSelectCamera();
            //Handler->readCallsign();
            //Handler->readGpsPosition();
            //Handler->readGpsTrack();
            Handler->readMagnetometer();
            //Handler->readAccelerometer();
            Handler->readGyroscope();
            //Handler->readCompassHeading();
            //Handler->readGpioDirection();
            //Handler->readGpioOutValue();
            //Handler->readGpioReadState();
            Handler->readDebuggingInfo();
            Handler->readBuildInfo();

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

