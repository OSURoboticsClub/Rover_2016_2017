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

AbstractController::JoystickState::JoystickState()
{
    std::fill(axes, axes + 8, 0.f);
    std::fill(buttons, buttons + 32, false);
}


void AbstractController::emitChanges()
{
    //m_file->readData()
    if(read(m_file->handle(), &m_jse, sizeof(m_jse)) > 0){
        if(m_jse.type == JS_EVENT_AXIS){
            m_currentState->axes[m_jse.number] = m_jse.value;
            emitAxisChanges(m_jse.value);
        } else if(m_jse.type == JS_EVENT_BUTTON){
            m_currentState->buttons[m_jse.number] = m_jse.value;
            emitButtonChanges(m_jse.value);
        }
    }
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

