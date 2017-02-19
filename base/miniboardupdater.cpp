#include "miniboardupdater.h"

MiniBoardUpdater::MiniBoardUpdater(QObject *parent)
    : QThread(parent)
{
    //runTime.start();
    connect(this, SIGNAL(update()), SerialHandler::instance()->p(), SLOT(readBatteryVoltage()));
}

void MiniBoardUpdater::run()
{

    qDebug() << "Update timer on";
    updateAll();
    qDebug() << "Update timer off";
    m_run = true;
}

void MiniBoardUpdater::updateAll()
{
    while(m_run){
        emit update();
        //if runtime has gone 1000 ms more
        //if ((runTime.elapsed() % 5000) == 0){
            //qDebug() << "updating UI";
            //TODO: pull all things UI needs to display from robot
            //Handler->readPause();
            msleep(1500);
            //SerialHandler::instance()->p()->readBatteryVoltage();
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

