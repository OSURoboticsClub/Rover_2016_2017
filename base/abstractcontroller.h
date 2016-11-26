#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>


/* TODO: fill in this abstract class with virtual methods that correspond to
 * generic controls for a game controller.
 * https://github.com/OregonStateMarsRover/2011
 */
class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = 0);
    virtual double axisRightX() = 0;
    virtual double axisRightY() = 0;
    virtual double axisLeftX() = 0;
    virtual double axisLeftY() = 0;
    virtual bool  buttonA() = 0;
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

signals:
    void axisRightYChanged(double value);
    void axisRightXChanged(double value);
    void axisLeftXChanged(double value);
    void axisLeftYChanged(double value);
    void buttonAChanged(bool value);
    void buttonBChanged(bool value);
    void buttonXChanged(bool value);
    void buttonYChanged(bool value);
    void buttonL1Changed(bool value);
    void buttonL2Changed(double value);
    void buttonL3Changed(bool value);
    void buttonR1Changed(bool value);
    void buttonR2Changed(double value);
    void buttonR3Changed(bool value);
    void buttonDownChanged(bool value);
    void buttonUpChanged(bool value);
    void buttonLeftChanged(bool value);
    void buttonRightChanged(bool value);
    void buttonStartChanged(bool value);
    void buttoSelectChanged(bool value);
    void buttonCenterChanged(bool value);

public slots:
};

#endif // ABSTRACTCONTROLLER_H
