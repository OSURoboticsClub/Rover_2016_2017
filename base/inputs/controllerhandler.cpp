#include "controllerhandler.h"

#include <QDebug>
#include <algorithm>
#include "serial/serialhandler.h"

/* TO Be used for updating the controllers and handling. The core functionality
 * should be moved to the AbstractController Class. The AbstractController class
 * should have all the commands for converting data and sending it to the rover.
 * Additionally, the Controller classes will have functions that will update
 * the controller states and emit changes to the rover.
 * So,
 * void emitChanges()
 *      if(i == 1) {
 *          convert and send packet
 *      }
 * .
 */

ControllerHandler::ControllerHandler(QObject *parent)
    : QThread(parent),
      m_stop(false)
{

}

ControllerHandler::~ControllerHandler()
{
    delete m_controllers;
}


void ControllerHandler::run()
{
    resetControllers();
    eventLoop();
    qDebug() << "exciting controller handler";
}

void ControllerHandler::stop()
{
    qDebug() << "quitting ControllerHandler thread";
    m_stop = true;
}

int ControllerHandler::controllerCount() {
    int count = 0;
    sf::Joystick::update();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++) {
        if(sf::Joystick::isConnected(i)) count++;
    }
    return count;
}

void ControllerHandler::eventLoop() {
    qDebug() << "entering ControllerHandler event loop";
    while(!m_stop) {
        if(m_controllerCount != controllerCount()) resetControllers();
        for(int i = 0; i < std::min(m_usableControllerCount, m_maxUsableControllers); i++) {
            (*m_controllers)[i]->emitChanges();
        }
        msleep(100);
    }
}

void ControllerHandler::resetControllers() {
    qDebug() << "resetting controllers";
    m_controllers = new QList<ControllerPointer>();
    m_controllerCount = controllerCount();
    for(int i = 0; i < m_controllerCount; i++) {
        if(sf::Joystick::isConnected(i)) {
            sf::Joystick::Identification id = sf::Joystick::getIdentification(i);
            qDebug() << "identified joystick with product id: " << id.productId;
            if(id.productId == 1025) {
                m_controllers->push_back(ControllerPointer (new XboxController(i)));
            } else if(id.productId == 22288) {
                m_controllers->push_back(ControllerPointer (new FrSky(i)));
            }
        }
    }
    m_usableControllerCount = m_controllers->size();

}
