#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H


#include "abstractcontroller.h"

/*
////////////////////////////////////////////////////////////
// Additions by PotatoSoftware
////////////////////////////////////////////////////////////
#define BUTTON_A sf::Joystick::X // A button on controller
#define BUTTON_X sf::Joystick::Z // X button on controller
#define BUTTON_B sf::Joystick::Y // B button on controller
#define BUTTON_Y sf::Joystick::R // Y button on controller
#define BUTTON_LB sf::Joystick::U // Left bumper on controller
#define BUTTON_RB sf::Joystick::V // Right bumper on controller
#define AXIS_LEFT_X sf::Joystick::Axis::X //  Left stick x value
#define AXIS_LEFT_Y sf::Joystick::Axis::Y // Left stick y value
#define AXIS_RIGHT_X sf::Joystick::Axis::U // Right stick x value
#define AXIS_RIGHT_Y sf::Joystick::Axis::R // Right stick y value
#define TRIGGERS sf::Joystick::Axis::Z // Positive Z = LT, and Negative Z = RT
*/

// https://www.reddit.com/r/sfml/comments/1huq2v/i_dont_know_if_this_helps_but_i_created_button/


class XboxController : public AbstractController
{
    Q_OBJECT
public:
    explicit XboxController(int id, QObject *parent = 0);
    ~XboxController();
    virtual void emitChanges();

    virtual double axisLeftX();
    virtual double axisLeftY();
    virtual double axisRightX();
    virtual double axisRightY();

    bool buttonA();
    bool buttonB();
    bool buttonX();
    bool buttonY();
    // TODO: fully implement these
    /*
    bool buttonL1();
    bool buttonL3();
    double buttonL2();
    bool buttonR1();
    bool buttonR3();
    double buttonR2();
    bool buttonUp();
    bool buttonDown();
    bool buttonLeft();
    bool buttonRight();
    bool buttonStart();
    bool buttonSelect();
    bool buttonCenter();
*/
signals:
    virtual void axisLeftXChanged(double value);
    virtual void axisLeftYChanged(double value);
    virtual void axisRightXChanged(double value);
    virtual void axisRightYChanged(double value);

    void buttonAChanged(bool value);
    void buttonBChanged(bool value);
    void buttonXChanged(bool value);
    void buttonYChanged(bool value);

private:
    enum class axisMappings {
        LEFT_X = sf::Joystick::Axis::X,
        LEFT_Y = sf::Joystick::Axis::Y,
        RIGHT_X = sf::Joystick::Axis::U,
        RIGHT_Y = sf::Joystick::Axis::R,
    };
    enum class buttonMappings {
        A = sf::Joystick::X,
        B = sf::Joystick::Y,
        X = sf::Joystick::Z,
        Y = sf::Joystick::R,
    };
};

#endif // XBOXCONTROLLER_H
