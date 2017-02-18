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
            if(id.productId == 1025) { // TODO: actual values here
                m_controllers->push_back(ControllerPointer (new XboxController(i)));
            } else if(id.productId == 22288) {
                m_controllers->push_back(ControllerPointer (new FrSky(i)));
            } else if(id.productId == 49686) {
                m_controllers->push_back(ControllerPointer (new Ps3(i)));
            }
        }
    }
    m_usableControllerCount = m_controllers->size();
    connectControllers();

}

void ControllerHandler::connectControllers()
{
    // and connect them
    if(m_usableControllerCount > 0 && m_maxUsableControllers > 0) {
        connectDriveController(m_controllers->at(0));
    }
    if(m_usableControllerCount > 1 && m_maxUsableControllers > 1) {
        // TODO: connect other input controller
    } // etc.
}

void ControllerHandler::connectDriveController(ControllerPointer controller) {
    connect(&(*controller), SIGNAL(axisYChanged(double,double)),
            this, SLOT(sendDriveMotorPower(double,double)));
}

void ControllerHandler::sendDriveMotorPower(double left, double right)
{
    /* TODO: fix right value */
    double conversionFactor = -1.27;
    left *= conversionFactor;
    right *= conversionFactor;
    int8_t l_drive = static_cast<int8_t>(left);
    int8_t r_drive = static_cast<int8_t>(right);
    qDebug() << "writing motor power" << l_drive << r_drive;
    SerialHandler::instance()->p()->writeDriveMotorPower(
        l_drive, l_drive, l_drive, r_drive, r_drive, r_drive
    );
}
