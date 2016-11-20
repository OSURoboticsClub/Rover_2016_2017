/*
 * FrSkyGamepad.cpp - part of FrSkyGamepad unit
 * copyright (c) 2011-2012, fluxrobotics
 */

#include "frsky.h"

const sf::Joystick::Axis FrSky::mapAxesNums[] = {
   sf::Joystick::X,
   sf::Joystick::Y,
   sf::Joystick::Z,
   sf::Joystick::R,
   sf::Joystick::U,
   sf::Joystick::V,
   sf::Joystick::PovX,
   sf::Joystick::PovY,
};

int FrSky::getRawAxisCount()
{
    const int NUM_AXES = (int)(sizeof(mapAxesNums)/sizeof(mapAxesNums[0]));
    return NUM_AXES;
}

bool FrSky::initInput(int joystickId)
{
    //see docs. SFML provides only static functions for Joystick class
    //so no initialization for hardware is required

    m_joystickId = joystickId;

    //m_f_haveJoystick = sf::Joystick::isConnected(m_joystickId);
    m_f_haveJoystick = true;

    return true;
}

bool FrSky::releaseInput()
{
    m_joystickId = 0;
    m_f_haveJoystick = false;

    // nothing here yet

    return true;
}

bool FrSky::updateState()
{
    if (!m_f_haveJoystick)
        return false;

    sf::Joystick::update();

    m_leftx  = this->getScaledAxis(sf::Joystick::X);
    m_lefty  = this->getScaledAxis(sf::Joystick::Y);

    // there is uncertainty in Joystick axes, try one of these
    m_righty  = this->getScaledAxis(sf::Joystick::R);
    m_rightx  = this->getScaledAxis(sf::Joystick::Z);


    for (int i = 0; i < 32; i++)
    {
        m_joyKeyState[i] = sf::Joystick::isButtonPressed(m_joystickId,i);
    }

    return true;
}




