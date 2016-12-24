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
    void buttonAChanged(bool value);
    void buttonBChanged(bool value);
    void buttonXChanged(bool value);
    void buttonYChanged(bool value);

protected:
    static const int BUTTON_A = 0;
    static const int BUTTON_B = 1;
    static const int BUTTON_X = 2;
    static const int BUTTON_Y = 3;

    virtual void emitAxisChanges(int axisIndex, double value);
    virtual void emitButtonChanges(int buttonIndex, bool value);
};

#endif // XBOXCONTROLLER_H
