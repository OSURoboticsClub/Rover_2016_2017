#include "controllerhandler.h"

#include <QDebug>
#include <algorithm>
#include <QString>
#include "serial/serialhandler.h"
#include <linux/joystick.h>

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
      m_stop(false),
      m_controllers(new QList<ControllerPointer>())
{

}

ControllerHandler::~ControllerHandler()
{
    delete m_controllers;
    delete frSky;
}


void ControllerHandler::run()
{
    setControllers();
    eventLoop();
    m_stop = false;
    qDebug() << "exciting controller handler";
}

void ControllerHandler::stop()
{
    qDebug() << "quitting ControllerHandler thread";
    m_stop = true;
}


void ControllerHandler::eventLoop()
{
    qDebug() << "entering ControllerHandler event loop";
    emit changeButtonColor("#169d06", true);
    while(!m_stop) {

        for(int i = 0; i < m_controllers->size(); i++) {
            (*m_controllers)[i]->emitChanges();
        }
        msleep(250);
    }
    emit changeButtonColor("#9d0606", false);
}

void ControllerHandler::setControllers() {
    qDebug() << "setting controllers";
    m_controllers->clear();
    for(int i = 0; i < 32; i++){
        QString fname = QString("/dev/input/js%1").arg(i);
        QFile *file = new QFile(fname);
        if(file->open(QIODevice::ReadOnly)){
            char c_name[128];
            ioctl(file->handle(), JSIOCGNAME(sizeof(c_name)), c_name);
            QString name = c_name;
            if(name.startsWith("FrSky")){
                frSky = new FrSky(file);
                m_controllers->push_back(ControllerPointer(frSky));
            }
        }
      }
}
