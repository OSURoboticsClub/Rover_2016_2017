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

protected:
    void emitAxisChanges(int axisIndex);
    void emitButtonChanges(int buttonIndex);
};

#endif // XBOXCONTROLLER_H
