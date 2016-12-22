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
<<<<<<< HEAD
    explicit AbstractController(int id, QObject *parent = 0);
    ~AbstractController();
    virtual void emitChanges() = 0;
    int getPriority() {return priority;}

    virtual double axisLeftX() = 0;
    virtual double axisLeftY() = 0;
    virtual double axisRightX() = 0;
    virtual double axisRightY() = 0;
=======
    explicit AbstractController(QString port, QObject *parent = 0);
    virtual ~AbstractController();
    virtual double axisRightX() = 0;
    virtual double axisRightY() = 0;
    virtual double axisLeftX() = 0;
    virtual double axisLeftY() = 0;
    virtual bool buttonA() = 0;
    virtual bool buttonB() = 0;
    virtual bool buttonY() = 0;
    virtual bool buttonX() = 0;
    virtual bool buttonL1() = 0;
    virtual double buttonL2() = 0;
    virtual bool buttonL3() = 0;
    virtual bool buttonR1() = 0;
    virtual double buttonR2() = 0;
    virtual bool buttonR3() = 0;
    virtual bool buttonUp() = 0;
    virtual bool buttonDown() = 0;
    virtual bool buttonLeft() = 0;
    virtual bool buttonRight() = 0;
    virtual bool buttonStart() = 0;
    virtual bool buttonSelect() = 0;
    virtual bool buttonCenter() = 0;
>>>>>>> 91ddf59f49015932795badb5f5e9d4fc164fcab2

signals:
    virtual void axisLeftXChanged(double value) = 0;
    virtual void axisLeftYChanged(double value) = 0;
    virtual void axisRightXChanged(double value) = 0;
    virtual void axisRightYChanged(double value) = 0;

protected:
    int id;
    float axisTolerance = 0;
    int priority;

<<<<<<< HEAD
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

=======
public slots:
>>>>>>> 91ddf59f49015932795badb5f5e9d4fc164fcab2
};

#endif // ABSTRACTCONTROLLER_H
