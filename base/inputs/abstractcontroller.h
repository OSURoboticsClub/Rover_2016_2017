#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <SFML/Window/Joystick.hpp>

typedef ::sf::Joystick js;

/*
 *
 */
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

    void axisXChanged(double left, double right);
    void axisYChanged(double left, double right);

protected:
    virtual void emitAxisChanges(int axisIndex, double value);
    virtual void emitButtonChanges(int buttonIndex, bool value);

    // TODO : change this to a struct possible
    static const int AXIS_LEFT_Y = 1;
    static const int AXIS_LEFT_X = 2;
    static const int AXIS_RIGHT_Y = 3;
    static const int AXIS_RIGHT_X = 4;

    int m_id;
    float m_axisTolerance;
    int m_priority;

    // Copied and adapted from SFML/Window/JoystickImpl.hpp
    struct JoystickState {
        JoystickState();
        float axes[js::AxisCount];      ///< Position of each axis, in range [-100, 100]
        bool  buttons[js::ButtonCount]; ///< Status of each button (true = pressed)
    };
    JoystickState *m_currentState;
};

#endif // ABSTRACTCONTROLLER_H
