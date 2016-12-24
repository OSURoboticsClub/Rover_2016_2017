#include "controllerhandler.h"



ControllerHandler::ControllerHandler(QObject *parent)
    : QObject(parent)
{

}

ControllerHandler::~ControllerHandler()
{
    delete controllers;
}

int ControllerHandler::getControllerCount() {
    int count = 0;
    for(unsigned int i = 0; i < sf::Joystick::Count; i++) {
        if(sf::Joystick::isConnected(i)) count++;
    }
    return count;
}

void ControllerHandler::eventLoop() {
    // TODO: while some better condition
    while(1) {
        if(controllerCount != getControllerCount()) resetControllers();
        // TODO: implement max usable controllers instead of controllerCount
        for(int i = 0; i < std::min(controllerCount, maxUsableControllers); i++) {
            (*controllers)[i]->emitChanges();
        }
    }
}

void ControllerHandler::resetControllers() {
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
              { return a->priority() < b->priority(); }
    );
    // and connect them
}

void ControllerHandler::connectControllers()
{
    if(controllerCount > 0 && maxUsableControllers > 0) {
        connectDriveController(controllers->at(0));
    }
}

void ControllerHandler::connectDriveController(ControllerPointer controller) {
    // TODO

}

void ControllerHandler::start()
{
    resetControllers();
    eventLoop();
}
