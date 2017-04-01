#include "abstractcontroller.h"

#include <unistd.h>

#include <algorithm>
#include <QDebug>

#include "serial/serialhandler.h"


AbstractController::AbstractController(QFile *file, QObject *parent)
    : QObject(parent),
      m_file(file),
      m_axisTolerance(0),
      m_currentState(new JoystickState())
{
}

AbstractController::~AbstractController()
{
}

AbstractController::JoystickState::JoystickState()
{
    std::fill(axes, axes + 8, 0.f);
    std::fill(buttons, buttons + 32, false);
}

void AbstractController::emitChanges()
{
    while(read(m_file->handle(), &m_jse, sizeof(m_jse)) > 0){
        if((m_jse.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS){
            m_previousState->axes[m_jse.number] = m_currentState->axes[m_jse.number];
            m_currentState->axes[m_jse.number] = m_jse.value;
            qDebug("Axis: %i, value: %i", m_jse.number, m_jse.value);
            emitAxisChanges(m_jse.number);
        } else if((m_jse.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON){
            qDebug("Button: %i, value: %i", m_jse.number, m_jse.value);
            m_previousState->buttons[m_jse.number] = m_currentState->buttons[m_jse.number];
            m_currentState->buttons[m_jse.number] = m_jse.value;
            emitButtonChanges(m_jse.number);
        }
    }
}



void AbstractController::sendArmMotorPower(qint16 motor1, qint16 motor2, qint16 motor3, qint16 motor4, qint16 motor5){
    qint16 vel[5] = {motor1, motor2, motor3, motor4, motor5};
    for(int i = 0; i < 5; i++) {
        vel[i] /= m_armControl->velocityScaling;
        vel[i] /= 255;
    }

    qint8 m[5];
    for(int i = 0; i < 5; i++) {
        qint8 v = static_cast<qint8>(vel[i]);
        m[i] = m_armControl->positions[i] + v;
    }

    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeArmMotors",
                              Q_ARG( signed char, m[0]),
                              Q_ARG( signed char, m[1] ),
                              Q_ARG( signed char, m[2] ),
                              Q_ARG( signed char, m[3] ),
                              Q_ARG( signed char, m[4] ));
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
    qDebug("%i",swerve_state);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeSwerveDriveState",
                              Q_ARG( unsigned char, swerve_state ));
}

void AbstractController::sendPauseState(qint16 pauseValue){
    double conversionFactor = 1;  // change depending on controller mapping of switch
    uint8_t pauseState =  static_cast<uint8_t>(pauseValue * conversionFactor);
    qDebug("Send Pause: %i",pauseState);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writePause",
                              Q_ARG( unsigned char, pauseState ));
}

void AbstractController::sendSelectCamera(qint16 increment)
{
    //Increments up if true, and down if false.
    //Loops around to other end if necessary.
<<<<<<< Updated upstream
    if(increment){
        if(m_camera_state < 6)
            m_camera_state++;
        else
            m_camera_state = 1;
    }
    else{
        if(m_camera_state > 1)
            m_camera_state--;
        else
            m_camera_state = 6;

    }
=======

        m_cameraState += increment;
        if(m_cameraState > 6)
            m_cameraState = 0;
        else if(m_cameraState < 0)
            m_cameraState = 6;

>>>>>>> Stashed changes
    //option without looping around
    /*
    if(increment && m_camera_state < 6)
        m_camera_state++;
    else if(!increment && m_camera_state > 1)
        m_camera_state--;
    */
<<<<<<< Updated upstream
    uint8_t selected_camera = static_cast<uint8_t>(m_camera_state);
    QMetaObject::invokeMethod(SerialHandler::instance()->p(), "writeSelectCamera",
                              Q_ARG( signed char, selected_camera ));
=======

    QMetaObject::invokeMethod(SerialHandler::instance()->p(),
                              "writeSelectCamera",
                              Q_ARG(quint8, m_cameraState));
>>>>>>> Stashed changes
}


//sendCameraCommand
//sendServo?
//sendCallsign?
