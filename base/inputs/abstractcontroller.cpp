#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include "serial/serialhandler.h"
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

AbstractController::AbstractController(QFile *file, QObject *parent)
    : QObject(parent),
      m_file(file),
      m_axisTolerance(255),
      m_currentState(new JoystickState()),
      m_swerveControlInto(new SwerveControl()),
      m_swerveControlOutof(new SwerveControl())
{
}

AbstractController::~AbstractController()
{
    delete m_currentState;
    delete m_swerveControlInto;
    delete m_swerveControlOutof;
}

AbstractController::JoystickState::JoystickState()
{
    std::fill(axes, axes + 8, 0.f);
    std::fill(buttons, buttons + 32, false);
}

AbstractController::SwerveControl::SwerveControl()
{
    elapsed = new QTime;
}

void AbstractController::emitChanges()
{
    if(m_swerveControlInto->lock && (m_swerveControlInto->elapsed->elapsed() %  m_swerveControlInto->timeout) == m_swerveControlInto->timeout) {
        m_swerveControlInto->lock = false;
        sendDriveMotorPower(0, 0);
    }
    if(m_swerveControlOutof->lock && (m_swerveControlOutof->elapsed->elapsed() % m_swerveControlOutof->timeout) == m_swerveControlOutof->timeout){
        m_swerveControlOutof->lock = false;
        sendDriveMotorPower(0,0);
    }



    while(read(m_file->handle(), &m_jse, sizeof(m_jse)) > 0){
        if((m_jse.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS){
            if(abs(m_currentState->axes[m_jse.number] - m_jse.value) > m_axisTolerance) {
                m_currentState->axes[m_jse.number] = m_jse.value;
                qDebug("Axis: %i, value: %i", m_jse.number, m_jse.value);
                emitAxisChanges(m_jse.number);
            } else {
                m_currentState->axes[m_jse.number] = m_jse.value;
                break;
            }

        } else if((m_jse.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON){
            qDebug("Button: %i, value: %i", m_jse.number, m_jse.value);
            if(m_currentState->buttons[m_jse.number] == m_jse.value)
                break;
            m_currentState->buttons[m_jse.number] = m_jse.value;
            emitButtonChanges(m_jse.number);

        }
    }
}



void AbstractController::sendArmMotorPower(qint16 motor1, qint16 motor2, qint16 motor3, qint16 motor4, qint16 motor5){
    quint8 conversionFactor = 255;   //positive or negative tbd
    int8_t m1 = static_cast<int8_t>(motor1 / conversionFactor);
    int8_t m2 = static_cast<int8_t>(motor2 / conversionFactor);
    int8_t m3 = static_cast<int8_t>(motor3 / conversionFactor);
    int8_t m4 = static_cast<int8_t>(motor4 / conversionFactor);
    int8_t m5 = static_cast<int8_t>(motor5 / conversionFactor);

    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeArmMotors",
                              Q_ARG( signed char, m1 ),
                              Q_ARG( signed char, m2 ),
                              Q_ARG( signed char, m3 ),
                              Q_ARG( signed char, m4 ),
                              Q_ARG( signed char, m5 ));
}

void AbstractController::sendDriveMotorPower(qint16 left, qint16 right){
    quint8 conversionFactor = 255;
    int8_t l_drive = static_cast<int8_t>(left / conversionFactor);
    int8_t r_drive = static_cast<int8_t>(right / conversionFactor);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeDriveMotorPower",
                              Q_ARG( signed char, l_drive ),
                              Q_ARG( signed char, l_drive ),
                              Q_ARG( signed char, l_drive ),
                              Q_ARG( signed char, r_drive ),
                              Q_ARG( signed char, r_drive ),
                              Q_ARG( signed char, r_drive ));

}

void AbstractController::sendSwerveDriveState(qint16 swerveValue){
    uint8_t swerve_state = static_cast<uint8_t>(swerveValue);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeSwerveDriveState",
                              Q_ARG( unsigned char, swerve_state ));
}

void AbstractController::sendPauseState(qint16 pauseValue){
    double conversionFactor = 1;  // change depending on controller mapping of switch
    uint8_t pauseState =  static_cast<uint8_t>(pauseValue * conversionFactor);
    qDebug("Send Pause: %i",pauseState);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writePause",
                              Q_ARG( unsigned char, pauseState ));
    emit frSkyPaused(pauseState);
}

void AbstractController::sendSelectCamera(qint16 increment){
    //Increments up if true, and down if false.
    //Loops around to other end if necessary.
    if(increment) {
        if(m_camera_state < 6)
            m_camera_state++;
        else
            m_camera_state = 1;
    } else {
        if(m_camera_state > 1)
            m_camera_state--;
        else
            m_camera_state = 6;
    }
    //option without looping around
    /*
    if(increment && m_camera_state < 6)
        m_camera_state++;
    else if(!increment && m_camera_state > 1)
        m_camera_state--;
    */
    uint8_t selected_camera = static_cast<uint8_t>(m_camera_state);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeSelectCamera",
                              Q_ARG( unsigned char, selected_camera ));
}
void AbstractController::sendPanPrimary(quint16 _pan, quint16 _tilt){
    double conversion_factor = 16320.0;
    quint16 pan = static_cast<quint16>((_pan + m_cameraPan) / conversion_factor);
    quint16 tilt = static_cast<quint16>((_tilt + m_cameraTilt) / conversion_factor);

    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writePanTiltPrimary",
                              Q_ARG(quint16, pan),
                              Q_ARG(quint16, tilt));
}


void AbstractController::sendSwerveMotorPower(qint16 dir)
{
    qDebug() << "Writing motor power";
    qint8 modDir = 0;
    if (dir)
        modDir = -1;
    else if (!dir)
        modDir = 1;

    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeDriveMotorPower",
                              Q_ARG( signed char, 50 * modDir),
                              Q_ARG( signed char, 0 ),
                              Q_ARG( signed char, 50 * modDir),
                              Q_ARG( signed char, 50 * modDir),
                              Q_ARG( signed char, 0 ),
                              Q_ARG( signed char, 50 * modDir));
    if (dir) {
        m_swerveControlOutof->elapsed->start();
        m_swerveControlOutof->lock = true;
    }
    else if (dir) {
        m_swerveControlInto->elapsed->start();
        m_swerveControlInto->lock = true;
    }
}
/*
void AbstractController::sendPanSecondary(qint16 _pan2, qint16 _tilt2){
    quint8 conversion_factor = 255;
    int8_t pan2 = static_cast<uint8_t>(_pan2/conversion_factor);
    int8_t tilt2 = static_cast<uint8_t>(_tilt2/conversion_factor);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writePanTiltSecondary",
                              Q_ARG(signed char, pan2),
                              Q_ARG(signed char, tilt2));
}
*/

//sendCameraCommand
//sendServo?
//sendCallsign?
