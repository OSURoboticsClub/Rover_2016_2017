#include "xboxcontroller.h"

#include <QDebug>

XboxController::XboxController(QString port, QObject *parent) : AbstractController(port, parent)
{
    QList<int> gamepads = QGamepadManager::instance()->connectedGamepads();
    QList<int>::const_iterator i;
    for(i = gamepads.begin(); i != gamepads.end(); ++i) {
        /*
        if(*i->name() == port) {
            m_gamepad = i;
        }
        */
    }
    qDebug() << "Could not identify port";
}


XboxController::~XboxController()
{
    delete m_gamepad;
}

