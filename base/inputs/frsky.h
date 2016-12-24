#ifndef FRSKY_H
#define FRSKY_H

#include "abstractcontroller.h"

/*
 * TODO: This class needs to be fully implemented, similar to the
 */

class FrSky : public AbstractController
{
    Q_OBJECT
public:
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

#endif // FRSKY_H
