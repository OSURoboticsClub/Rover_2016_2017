#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include "serial/serialhandler.h"
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

AbstractController::AbstractController(QFile *file, QObject *parent)
    : QObject(parent),
      m_file(file),
      m_axisTolerance(0)
{
    // m_mode = static_cast<int>(js::isButtonPressed(m_id, 1));
}

AbstractController::~AbstractController()
{
}

void AbstractController::emitChanges()
{
    //m_file->readData()
    read(m_file->handle(), &m_jse, sizeof(m_jse));
}

void AbstractController::emitAxisChanges(int axisIndex)
{

}

void AbstractController::emitButtonChanges(int buttonIndex)
{

}


void AbstractController::sendArmMotorPower(double motor1, double motor2, double motor3, double motor4, double motor5){
    double conversionFactor = 1.27;
    int8_t arm_motor_1 = static_cast<int8_t>(motor1 * conversionFactor);
    int8_t arm_motor_2 = static_cast<int8_t>(motor2 * conversionFactor);
    int8_t arm_motor_3 = static_cast<int8_t>(motor3 * conversionFactor);
    int8_t arm_motor_4 = static_cast<int8_t>(motor4 * conversionFactor);
    int8_t arm_motor_5 = static_cast<int8_t>(motor5 * conversionFactor);
    SerialHandler::instance()->p()->writeArmMotors(arm_motor_1, arm_motor_2, arm_motor_3, arm_motor_4, arm_motor_5);
}

