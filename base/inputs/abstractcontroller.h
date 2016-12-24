#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <SFML/Window/Joystick.hpp>
#include <algorithm>


// http://www.sfml-dev.org/documentation/2.4.1/classsf_1_1Joystick.php
// http://stackoverflow.com/questions/22888134/library-to-get-input-from-a-ps4-or-xbox-controller


// TODO: currently this works but is overly verbose. How could this
// be improved?
class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(int id, QObject *parent = 0);
    ~AbstractController();
    virtual void emitChanges();

    int priority() { return m_priority; }

    double axisLeftX() const;
    double axisLeftY() const;
    double axisRightX() const;
    double axisRightY() const;

signals:
    void axisLeftXChanged(double value);
    void axisLeftYChanged(double value);
    void axisRightXChanged(double value);
    void axisRightYChanged(double value);

protected:
    virtual void emitAxisChanges(int axisIndex, double value);
    virtual void emitButtonChanges(int buttonIndex, bool value);

    static const int AXIS_LEFT_X = 0;
    static const int AXIS_LEFT_Y = 1;
    static const int AXIS_RIGHT_X = 2;
    static const int AXIS_RIGHT_Y = 3;


    int m_id;
    float m_axisTolerance;
    int m_priority;


    // Copied and adapted from SFML/Window/JoystickImpl.hpp
    struct JoystickState {
        JoystickState()
        {
            std::fill(axes, axes + sf::Joystick::AxisCount, 0.f);
            std::fill(buttons, buttons + sf::Joystick::ButtonCount, false);
        }
        float axes[sf::Joystick::AxisCount];      ///< Position of each axis, in range [-100, 100]
        bool  buttons[sf::Joystick::ButtonCount]; ///< Status of each button (true = pressed)
    };
    JoystickState *m_currentState;
};

#endif // ABSTRACTCONTROLLER_H
