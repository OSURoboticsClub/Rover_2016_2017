#ifndef FRSKY_H
#define FRSKY_H

#include "abstractcontroller.h"

/*
 * TODO: This class needs to be fully implemented, similar to the
<<<<<<< HEAD
 * XboxController class.
=======
 * XboxController class
>>>>>>> 91ddf59f49015932795badb5f5e9d4fc164fcab2
 */
class FrSky : public AbstractController
{
    Q_OBJECT
public:
<<<<<<< HEAD
    explicit FrSky(int id, QObject *parent = 0);
    ~FrSky();
    virtual void emitChanges();

    virtual double axisLeftX();
    virtual double axisLeftY();
    virtual double axisRightX();
    virtual double axisRightY();

signals:
    virtual void axisLeftXChanged(double value);
    virtual void axisLeftYChanged(double value);
    virtual void axisRightXChanged(double value);
    virtual void axisRightYChanged(double value);

private:
    enum class axisMappings {
        LEFT_X = sf::Joystick::Axis::X,
        LEFT_Y = sf::Joystick::Axis::Y,
        RIGHT_X = sf::Joystick::Axis::U,
        RIGHT_Y = sf::Joystick::Axis::R,
    };
    enum class buttonMappings {

    };
};


=======
    explicit FrSky(QString port, QObject *parent = 0);
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

>>>>>>> 91ddf59f49015932795badb5f5e9d4fc164fcab2
#endif // FRSKY_H
