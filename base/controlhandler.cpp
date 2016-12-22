#include "controlhandler.h"

ControlHandler::ControlHandler(QString mainControllerPort, QObject *parent)
    : QObject(parent)
{
    mainController = new FrSky(mainControllerPort);
}

void ControlHandler::setMainControllerPort(QString port)
{
    mainController = new FrSky(port);
}

void ControlHandler::sendDriveMotorPower(int left_drive, int right_drive)
{
    send_drive_motor_power(left_drive, right_drive);
}

void ControlHandler::connectMainController()
{
    connect(&mainController, , this, sendDriveMotorPower);
}
