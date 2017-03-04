#include "miniboardupdater.h"

MiniBoardUpdater::MiniBoardUpdater(QObject *parent)
    : QThread(parent)
{

    //runTime.start();

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
    emit changeButtonColor("#169d06", true);
    while(m_run){
        //emit update();
        //if runtime has gone 1000 ms more
        qDebug() << "Pulling Reads";
        Handler->readBatteryVoltage();
        Handler->readDriveMotorPower();
        Handler->readSwerveDriveState();
        Handler->readArmMotors();
        Handler->readPotentiometers();
        Handler->readSelectCamera();
        Handler->readCallsign();
        Handler->readGpsPosition();
        //Handler->readGpsTrack();
        Handler->readMagnetometer();
        //Handler->readAccelerometer();
        Handler->readGyroscope();
        //Handler->readCompassHeading();
        Handler->readGpioDirection();
        Handler->readGpioOutValue();
        Handler->readGpioReadState();
        Handler->readDebuggingInfo();
        Handler->readBuildInfo();
        qDebug() << "Done Pulling";
        msleep(3000);

    }
    emit changeButtonColor("#9d0606", false);

}

void MiniBoardUpdater::stop()
{
    m_run = false;
}

int MiniBoardUpdater::timeRunning()
{
    return runTime.elapsed();
}

