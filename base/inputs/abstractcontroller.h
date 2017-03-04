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

signals:



protected:
    virtual void emitAxisChanges(int axisIndex);
    virtual void emitButtonChanges(int buttonIndex);
    void sendArmMotorPower(double m1, double m2, double m3, double m4, double m5);
    void sendDriveMotorPower(double l, double r);
    void sendPauseState(double p);
    void sendSwerveDriveState(double s);
    void sendSelectCamera(bool i);


    int m_id,m_camera_state = 1;
    float m_axisTolerance;
    int mode = 0;

    // Copied and adapted from SFML/Window/JoystickImpl.hpp
    struct JoystickState {
        JoystickState();
        float axes[js::AxisCount];      ///< Position of each axis, in range [-100, 100]
        bool  buttons[js::ButtonCount]; ///< Status of each button (true = pressed)
    };
    JoystickState *m_currentState;
};

#endif // ABSTRACTCONTROLLER_H
