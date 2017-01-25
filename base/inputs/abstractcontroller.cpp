#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include <cmath>
#include <algorithm>
#include <QDebug>

AbstractController::AbstractController(int id, QObject *parent)
    : QObject(parent),
      m_id(id),
      m_axisTolerance(0),
      m_priority(0)
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

            emitAxisChanges(i, axisPos);
        }
        m_currentState->axes[i] = axisPos;
    }

    for(unsigned int j = 0; j < js::ButtonCount; j++) {
        bool buttonPressed = js::isButtonPressed(m_id, j);
        if(buttonPressed != m_currentState->buttons[j]) {
            //emit ButtonChanges(j, buttonPressed);
            qDebug() <<"button is changed"<< j;

        }
        m_currentState->buttons[j] = buttonPressed;
    }

}

void AbstractController::emitAxisChanges(int axisIndex, double value)
{
    switch (axisIndex) {
    case AXIS_LEFT_X:
        emit axisLeftXChanged(value);

        emit axisXChanged(
                    value,
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_RIGHT_X))
                    );
        break;
    case AXIS_LEFT_Y:
        emit axisLeftYChanged(value);
        //double right = ;
        emit axisYChanged(
                    value,
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_RIGHT_Y))
                    );
        break;
    case AXIS_RIGHT_X:
        emit axisRightXChanged(value);
        emit axisXChanged(
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_LEFT_X)),
                    value
                    );
        break;
    case AXIS_RIGHT_Y:
        emit axisRightYChanged(value);
        emit axisYChanged(
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_LEFT_Y)),
                    value
                    );
        break;
    default:
        break;
    }
}

void AbstractController::emitButtonChanges(int buttonIndex, bool value)
{
    /*
    switch (buttonIndex) {
    case BUTTON_A:
        emit buttonAChanged(value);
        break;
    case BUTTON_B:
        emit buttonBChanged(value);
        emit axisYChanged(
                    value,
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_RIGHT_Y))
                    );
        break;
    case BUTTON_X:
        emit buttonXChanged(value);
        break;
    case BUTTON_Y:
        emit buttonYChanged(value);
        emit axisYChanged(
                    js::getAxisPosition(m_id,
                        static_cast<js::Axis>(AXIS_LEFT_Y)),
                    value
                    );
        break;
    default:
        break;
    }
    */
}

double AbstractController::axisLeftX() const
{
    return m_currentState->axes[AXIS_LEFT_X];
}

double AbstractController::axisLeftY() const
{
    return m_currentState->axes[AXIS_LEFT_Y];
}

double AbstractController::axisRightX() const
{
    return m_currentState->axes[AXIS_RIGHT_X];
}

double AbstractController::axisRightY() const
{
    return m_currentState->axes[AXIS_RIGHT_X];
}
