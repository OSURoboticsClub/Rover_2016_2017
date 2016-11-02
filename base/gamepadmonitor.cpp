/**
 * The GamepadMonitor class contains methods for interfacing with a gamepad.
 * This class will be used to interface with external controllers, and will
 * emit signals to send commands to rover.
 **/

#include <QtGamepad/QGamepad>

#include "gamepadmonitor.h"


GamepadMonitor::GamepadMonitor(QObject *parent)
    : QObject(parent),
      m_gamepad(0)
{
        auto gamepads = QGamepadManager::instance()->connectedGamepads();
        if (gamepads.isEmpty()) {
            return;
        }
        m_gamepad = new QGamepad(*gamepads.begin(), this);
}

GamepadMonitor::~GamepadMonitor()
{
    delete m_gamepad;
}
