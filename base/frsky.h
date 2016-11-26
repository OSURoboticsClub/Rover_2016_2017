#ifndef FRSKY_H
#define FRSKY_H

#include "abstractcontroller.h"

/*
 * TODO: This class needs to be fully implemented, similar to the
 * XboxController class
 */
class FrSky : public AbstractController
{
    Q_OBJECT
public:
    FrSky();
    ~FrSky();
    double axisRightX() {}
    double axisRightY() {}
    double axisLeftX() {}
    double axisLeftY() {}
    bool  buttonA() {}
    bool buttonB() {}
    bool buttonY() {}
    bool buttonX() {}
    bool buttonL1() {}
    bool buttonL3() {}
    double buttonL2() {}
    bool buttonR1() {}
    bool buttonR3() {}
    double buttonR2() {}
    bool buttonUp() {}
    bool buttonDown() {}
    bool buttonLeft() {}
    bool buttonRight() {}
    bool buttonStart() {}
    bool buttonSelect() {}
    bool buttonCenter() {}

private:

};

#endif // FRSKY_H
