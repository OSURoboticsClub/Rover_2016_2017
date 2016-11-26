#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <QGamepad>

#include "abstractcontroller.h"

/*
 * TODO: test this
 */

class XboxController : public AbstractController
{
    Q_OBJECT
public:
    XboxController();
    ~XboxController();
    double axisRightX() {return m_gamepad->axisRightX();}
    double axisRightY() {return m_gamepad->axisRightY();}
    double axisLeftX() {return m_gamepad->axisLeftX();}
    double axisLeftY() {return m_gamepad->axisLeftY();}
    bool  buttonA() {return m_gamepad->buttonA();}
    bool buttonB() {return m_gamepad->buttonB();}
    bool buttonY() {return m_gamepad->buttonY();}
    bool buttonX() {return m_gamepad->buttonX();}
    bool buttonL1() {return m_gamepad->buttonL1();}
    bool buttonL3() {return m_gamepad->buttonL3();}
    double buttonL2() {return m_gamepad->buttonL2();}
    bool buttonR1() {return m_gamepad->buttonR1();}
    bool buttonR3() {return m_gamepad->buttonR3();}
    double buttonR2() {return m_gamepad->buttonR2();}
    bool buttonUp() {return m_gamepad->buttonUp();}
    bool buttonDown() {return m_gamepad->buttonDown();}
    bool buttonLeft() {return m_gamepad->buttonLeft();}
    bool buttonRight() {return m_gamepad->buttonRight();}
    bool buttonStart() {return m_gamepad->buttonStart();}
    bool buttonSelect() {return m_gamepad->buttonSelect();}
    bool buttonCenter() {return m_gamepad->buttonCenter();}

private:
    QGamepad *m_gamepad;
};

#endif // XBOXCONTROLLER_H
