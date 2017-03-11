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
      m_axisTolerance(0)
{
    // m_mode = static_cast<int>(js::isButtonPressed(m_id, 1));
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
    //m_file->readData()
    if(read(m_file->handle(), &m_jse, sizeof(m_jse)) > 0){
        if(m_jse.type == JS_EVENT_AXIS){
            m_currentState->axes[m_jse.number] = m_jse.value;
            emitAxisChanges(m_jse.value);
        } else if(m_jse.type == JS_EVENT_BUTTON){
            m_currentState->buttons[m_jse.number] = m_jse.value;
            emitButtonChanges(m_jse.value);
        }
    }
}

void AbstractController::emitAxisChanges(int axisIndex)
{

}

void AbstractController::emitButtonChanges(int buttonIndex)
{

}


void AbstractController::sendArmMotorPower(double motor1, double motor2, double motor3, double motor4, double motor5){
    double conversionFactor = 1.27;  //positive or negative tbd
    int8_t arm_motor_1 = static_cast<int8_t>(motor1 * conversionFactor);
    int8_t arm_motor_2 = static_cast<int8_t>(motor2 * conversionFactor);
    int8_t arm_motor_3 = static_cast<int8_t>(motor3 * conversionFactor);
    int8_t arm_motor_4 = static_cast<int8_t>(motor4 * conversionFactor);
    int8_t arm_motor_5 = static_cast<int8_t>(motor5 * conversionFactor);
    SerialHandler::instance()->p()->writeArmMotors(arm_motor_1, arm_motor_2, arm_motor_3, arm_motor_4, arm_motor_5);
}

void AbstractController::sendDriveMotorPower(double left, double right){
    double conversionFactor = 1.27;  //positive or negative tbd
    int8_t l_f_drive = static_cast<int8_t>(left * conversionFactor);
    int8_t l_m_drive = static_cast<int8_t>(left * conversionFactor);
    int8_t l_b_drive = static_cast<int8_t>(left * conversionFactor);
    int8_t r_f_drive = static_cast<int8_t>(right * conversionFactor);
    int8_t r_m_drive = static_cast<int8_t>(right * conversionFactor);
    int8_t r_b_drive = static_cast<int8_t>(right * conversionFactor);
    SerialHandler::instance()->p()->writeDriveMotorPower(l_f_drive, l_m_drive, l_b_drive, r_f_drive, r_m_drive, r_b_drive);
}

void AbstractController::sendSwerveDriveState(double swerveValue){
    double conversionFactor = 1; //set depending on controller mapping of switch
    uint8_t swerve_state = static_cast<uint8_t>(swerveValue * conversionFactor);
    SerialHandler::instance()->p()->writeSwerveDriveState(swerve_state);
}

void AbstractController::sendPauseState(double pauseValue){
    double conversionFactor = 1;  // change depending on controller mapping of switch
    uint8_t pauseState =  static_cast<uint8_t>(pauseValue * conversionFactor);
    SerialHandler::instance()->p()->writePause(pauseState);
}

void AbstractController::sendSelectCamera(bool increment){
    //Increments up if true, and down if false.
    //Loops around to other end if necessary.
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
    //option without looping around
    /*
    if(increment && m_camera_state < 6)
        m_camera_state++;
    else if(!increment && m_camera_state > 1)
        m_camera_state--;
    */
    uint8_t selected_camera = static_cast<uint8_t>(m_camera_state);
    SerialHandler::instance()->p()->writeSelectCamera(selected_camera);
}

//sendCameraCommand
//sendServo?
//sendCallsign?
