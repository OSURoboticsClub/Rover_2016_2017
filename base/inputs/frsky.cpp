#include "frsky.h"


FrSky::FrSky(int id, QObject *parent)
    : AbstractController(id, parent)
{
}

FrSky::~FrSky()
{

}


FrSky::emitAxisChanges(int axisIndex){
    if(axisIndex == 1 || axisIndex == 2) {
        sendArmMotorPower();
    } else if (axisIndex == 3) {

    }
}


FrSky::emitButtonChanges(int buttonIndex){

}
