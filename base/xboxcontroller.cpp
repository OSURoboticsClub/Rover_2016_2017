#include "xboxcontroller.h"


XboxController::XboxController(int id, QObject *parent) : AbstractController(id, parent)
{

}

XboxController::~XboxController()
{

}

void XboxController::emitChanges()
{
    for(unsigned int i = 0; i < sf::Joystick::AxisCount; i++) {
        sf::Joystick::Axis axis = static_cast<sf::Joystick::Axis>(i);
        float axisPos = sf::Joystick::getAxisPosition(id, axis);
        if(std::abs(axisPos - currentState->axes[i]) > axisTolerance) {
            switch (axis) {
            case static_cast<int>(axisMappings::LEFT_X):
                emit axisLeftXChanged(axisPos);
                break;
            case static_cast<int>(axisMappings::LEFT_Y):
                emit axisLeftYChanged(axisPos);
                break;
            case static_cast<int>(axisMappings::RIGHT_X):
                emit axisRightXChanged(axisPos);
                break;
            case static_cast<int>(axisMappings::RIGHT_Y):
                emit axisRightYChanged(axisPos);
                break;
            default:
                break;
            }
        }
        currentState->axes[i] = axisPos;
    }

    for(unsigned int j = 0; j < sf::Joystick::ButtonCount; j++) {
        bool buttonPressed = sf::Joystick::isButtonPressed(id, j);
        if(buttonPressed != currentState->axes[j]) {
            switch (j) {
            case static_cast<int>(buttonMappings::A):
                emit buttonAChanged(buttonPressed);
                break;
            case static_cast<int>(buttonMappings::B):
                emit buttonBChanged(buttonPressed);
                break;
            case static_cast<int>(buttonMappings::X):
                emit buttonXChanged(buttonPressed);
                break;
            case static_cast<int>(buttonMappings::Y):
                emit buttonYChanged(buttonPressed);
                break;
            default:
                break;
            }
        }

        currentState->buttons[j] = buttonPressed;
    }
}

double XboxController::axisLeftX()
{
    return currentState->axes[static_cast<int>(axisMappings::LEFT_X)];
}

double XboxController::axisLeftY()
{
    return currentState->axes[static_cast<int>(axisMappings::LEFT_Y)];
}

double XboxController::axisRightX()
{
    return currentState->axes[static_cast<int>(axisMappings::RIGHT_X)];
}

double XboxController::axisRightY()
{
    return currentState->axes[static_cast<int>(axisMappings::RIGHT_Y)];
}

bool XboxController::buttonA()
{
    return currentState->axes[static_cast<int>(buttonMappings::A)];
}

bool XboxController::buttonB()
{
    return currentState->axes[static_cast<int>(buttonMappings::B)];
}

bool XboxController::buttonX()
{
    return currentState->axes[static_cast<int>(buttonMappings::X)];
}

bool XboxController::buttonY()
{
    return currentState->axes[static_cast<int>(buttonMappings::Y)];
}
