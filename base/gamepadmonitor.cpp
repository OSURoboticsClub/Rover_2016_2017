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
