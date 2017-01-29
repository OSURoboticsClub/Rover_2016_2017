#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H


#include "abstractcontroller.h"


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
    void emitAxisChanges(int axisIndex, double value);
    void emitButtonChanges(int buttonIndex, bool value);
};

#endif // XBOXCONTROLLER_H
