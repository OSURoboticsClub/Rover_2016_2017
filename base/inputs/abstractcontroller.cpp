#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include <cmath>
#include <algorithm>
#include <QDebug>


AbstractController::AbstractController(int id, QObject *parent)
    : QObject(parent),
      m_id(id),
      m_axisTolerance(0)
{
    m_currentState = new JoystickState();
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
    if(axisIndex == 1) {
        // stuff here
    }
}

void AbstractController::emitButtonChanges(int buttonIndex)
{

}
