#include "controlhandler.h"



ControlHandler::ControlHandler(QObject *parent)
    : QObject(parent)
{

}

ControlHandler::~ControlHandler()
{
    delete controllers;
}

int ControlHandler::getControllerCount() {
    int count = 0;
    for(unsigned int i = 0; i < sf::Joystick::Count; i++) {
        if(sf::Joystick::isConnected(i)) count++;
    }
    return count;
}

void ControlHandler::eventLoop() {
    // TODO: while some better condition
    while(1) {
        if(controllerCount != getControllerCount()) resetControllers();
        // TODO: implement max usable controllers instead of controllerCount
        for(int i = 0; i < std::min(controllerCount, maxUsableControllers); i++) {
            (*controllers)[i]->emitChanges();
        }
    }
}

void ControlHandler::resetControllers() {
    controllers = new QList<ControllerPointer>();
    controllerCount = getControllerCount();
    for(int i = 0; i < controllerCount; i++) {
        if(!sf::Joystick::isConnected(i)) {
            sf::Joystick::Identification id = sf::Joystick::getIdentification(i);
            if(id.productId == 0) { // TODO: actual values here
                controllers->push_back(ControllerPointer (new XboxController(i)));
            } else if(id.productId == 1) {
                controllers->push_back(ControllerPointer (new FrSky(i)));
            }
        }
    }

    std::sort(controllers->begin(), controllers->end(),
              [](ControllerPointer a, ControllerPointer b) -> bool
              { return a->getPriority() < b->getPriority(); }
    );
    // and connect them
}

void ControlHandler::connectControllers()
{
    if(controllerCount > 0 && maxUsableControllers > 0) {
        connectDriveController(controllers->at(0));
    }
}

void ControlHandler::connectDriveController(ControllerPointer controller) {
    // TODO

}

void ControlHandler::start()
{
    resetControllers();
    eventLoop();
}
