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
    double axisRightY() const;
    double axisRightX();
    double axisLeftX();
    double axisLeftY();
    bool  buttonA();
    bool buttonB();
    bool buttonY();
    bool buttonX();
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
    bool buttonselect();
    bool buttonCenter();

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
