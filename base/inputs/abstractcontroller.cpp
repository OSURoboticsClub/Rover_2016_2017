#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include "serial/serialhandler.h"
#include <cmath>
#include <algorithm>
#include <QDebug>


AbstractController::AbstractController(int id, QObject *parent)
    : QObject(parent),
      m_id(id),
      m_axisTolerance(0)
{
    m_currentState = new JoystickState();
    mode = static_cast<int>(js::isButtonPressed(m_id, 1));
}

AbstractController::JoystickState::JoystickState()
{
    std::fill(axes, axes + js::AxisCount, 0.f);
    std::fill(buttons, buttons + js::ButtonCount, false);
}

AbstractController::~AbstractController()
{
    delete m_currentState;
}

void AbstractController::emitChanges()
{
    for(unsigned int i = 0; i < js::AxisCount; i++) {
        float axisPos = js::getAxisPosition(m_id, static_cast<js::Axis>(i));
        if(std::abs(axisPos - m_currentState->axes[i]) > m_axisTolerance) {
            qDebug() << "axis changed: " << i;
            emitAxisChanges(i);
        }
        m_currentState->axes[i] = axisPos;
    }

    for(unsigned int j = 0; j < js::ButtonCount; j++) {
        bool buttonPressed = js::isButtonPressed(m_id, j);
        if(buttonPressed != m_currentState->buttons[j]) {
            //emit ButtonChanges(j, buttonPressed);
            emitButtonChanges(j);
            qDebug() <<"button is changed"<< j;

        }
        m_currentState->buttons[j] = buttonPressed;
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

