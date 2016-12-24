#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <SFML/Window/Joystick.hpp>
#include <algorithm>


// http://www.sfml-dev.org/documentation/2.4.1/classsf_1_1Joystick.php
// http://stackoverflow.com/questions/22888134/library-to-get-input-from-a-ps4-or-xbox-controller


// TODO: currently this works but is overly verbose. How could this
// be improved?
class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(int id, QObject *parent = 0);
    ~AbstractController();
    virtual void emitChanges() = 0;
    int getPriority() {return priority;}

    virtual double axisLeftX() = 0;
    virtual double axisLeftY() = 0;
    virtual double axisRightX() = 0;
    virtual double axisRightY() = 0;

signals:
    virtual void axisLeftXChanged(double value) = 0;
    virtual void axisLeftYChanged(double value) = 0;
    virtual void axisRightXChanged(double value) = 0;
    virtual void axisRightYChanged(double value) = 0;

protected:
    int id;
    float axisTolerance = 0;
    int priority;

    // Copied and adapted from SFML/Window/JoystickImpl.hpp
    struct JoystickState {
        JoystickState()
        {
            connected = false;
            std::fill(axes, axes + sf::Joystick::AxisCount, 0.f);
            std::fill(buttons, buttons + sf::Joystick::ButtonCount, false);
        }

        bool  connected;                          ///< Is the joystick currently connected?
        float axes[sf::Joystick::AxisCount];      ///< Position of each axis, in range [-100, 100]
        bool  buttons[sf::Joystick::ButtonCount]; ///< Status of each button (true = pressed)
    };
    JoystickState *currentState;
};

#endif // ABSTRACTCONTROLLER_H
