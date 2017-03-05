#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include "serial/serialhandler.h"
#include <cmath>
#include <algorithm>
#include <QDebug>


AbstractController::AbstractController(int id, QObject *parent)
    : QObject(parent),
      m_id(id),
      m_axisTolerance(0)
{
    m_currentState = new JoystickState();
    mode = static_cast<int>(js::isButtonPressed(m_id, 1));
}

AbstractController::JoystickState::JoystickState()
{
    std::fill(axes, axes + js::AxisCount, 0.f);
    std::fill(buttons, buttons + js::ButtonCount, false);
}

AbstractController::~AbstractController()
{
    delete m_currentState;
}

void AbstractController::emitChanges()
{
    for(unsigned int i = 0; i < js::AxisCount; i++) {
        float axisPos = js::getAxisPosition(m_id, static_cast<js::Axis>(i));
        if(std::abs(axisPos - m_currentState->axes[i]) > m_axisTolerance) {
            qDebug() << "axis changed: " << i;
            emitAxisChanges(i);
        }
        m_currentState->axes[i] = axisPos;
    }

    for(unsigned int j = 0; j < js::ButtonCount; j++) {
        bool buttonPressed = js::isButtonPressed(m_id, j);
        if(buttonPressed != m_currentState->buttons[j]) {
            //emit ButtonChanges(j, buttonPressed);
            emitButtonChanges(j);
            qDebug() <<"button is changed"<< j;

        }
        m_currentState->buttons[j] = buttonPressed;
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
