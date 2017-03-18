#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <QFile>
#include <linux/joystick.h>

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QFile *file, QObject *parent = 0);
    ~AbstractController();
    virtual void emitChanges();

signals:



protected:
    virtual void emitAxisChanges(quint8 axisIndex) = 0;
    virtual void emitButtonChanges(quint8 buttonIndex) = 0;
    void sendArmMotorPower(qint16 m1, qint16 m2, qint16 m3, qint16 m4, qint16 m5);
    void sendDriveMotorPower(qint16 l, qint16 r);
    void sendPauseState(qint16 p);
    void sendSwerveDriveState(qint16 s);
    void sendSelectCamera(qint16 i);


    QFile *m_file;
    struct js_event m_jse;
    int m_id;
    int m_camera_state = 1;
    int m_mode = 0;
    bool swerveStateON = false;
    float m_axisTolerance;

    struct JoystickState {
        JoystickState();
        qint16 axes[8];
        qint16 buttons[32];
    };
    JoystickState *m_currentState;



};

#endif // ABSTRACTCONTROLLER_H
