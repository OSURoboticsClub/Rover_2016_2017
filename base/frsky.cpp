#include "frsky.h"


FrSky::FrSky(int id, QObject *parent) : AbstractController(id, parent)
{

}

FrSky::~FrSky()
{

}

void FrSky::emitChanges()
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

            default:
                break;
            }
        }

        currentState->buttons[j] = buttonPressed;
    }
}

double FrSky::axisLeftX()
{
    return currentState->axes[static_cast<int>(axisMappings::LEFT_X)];
}

double FrSky::axisLeftY()
{
    return currentState->axes[static_cast<int>(axisMappings::LEFT_Y)];
}

double FrSky::axisRightX()
{
    return currentState->axes[static_cast<int>(axisMappings::RIGHT_X)];
}

double FrSky::axisRightY()
{
    return currentState->axes[static_cast<int>(axisMappings::RIGHT_Y)];
}
