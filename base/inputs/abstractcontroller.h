#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <QFile>
#include <linux/joystick.h>
#include <QTime>

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QFile *file, QObject *parent = 0);
    ~AbstractController();
    virtual void emitChanges();

signals:
    void frSkyPaused(qint16);

protected:
    virtual void emitAxisChanges(quint8 axisIndex) = 0;
    virtual void emitButtonChanges(quint8 buttonIndex) = 0;
    void sendArmMotorPower(qint16 m1, qint16 m2, qint16 m3, qint16 m4, qint16 m5);
    void sendDriveMotorPower(qint16 l, qint16 r);
    void sendPauseState(qint16 p);
    void sendSwerveDriveState(qint16 s);
    void sendSwerveMotorPower(qint16 direction);
    void sendSelectCamera(qint16 i);
    void sendPanPrimary(quint16,quint16);
  //  void sendPanSecondary(qint16,qint16);


    QFile *m_file;
    struct js_event m_jse;
    int m_id;
    int m_camera_state = 1;
    //middle of 2 byte unsigned
    quint16 m_cameraPan = 510;
    quint16 m_cameraTilt = 510;
    //TODO: use test tool to find max and min
    quint16 panMax = 800;
    quint16 panMin = 200;
    quint16 tiltMax = 800;
    quint16 tiltMin = 200;

    int m_mode = 0;


    struct SwerveControl {
        SwerveControl();
        QTime *elapsed;
        int timeout = 1000;
        bool lock = false;
    };

    SwerveControl *m_swerveControlInto;
    SwerveControl *m_swerveControlOutof;

    float m_axisTolerance;

    struct JoystickState {
        JoystickState();
        qint16 axes[8];
        qint16 buttons[32];
    };
    // TODO: no need for dynamic allocation
    JoystickState *m_pastState, *m_currentState;
};

#endif // ABSTRACTCONTROLLER_H
