#ifndef XINPUTGAMEPAD_H
#define XINPUTGAMEPAD_H

#include "include/SFML/Window.hpp"
#include "include/SFML/Window/Joystick.hpp"

class XInput
{
private:
    bool m_f_haveJoystick;
    bool m_joyKeyState[32];

    // axes - vertical, horizontal, rotation, throttle
    float m_lefty, m_leftx, m_righty, m_rightx,m_leftthrottle,m_rightthrottle,m_s1,m_s2;

    int m_joystickId;

    float getScaledAxis(sf::Joystick::Axis axisName)
    {
        if (!m_f_haveJoystick)
            return 0.0f;

        // scaling value. This is done to limit output below absolute value 1.0f
        const float JOYSTICK_SCALE = 1.0f/100.000001f;
        return sf::Joystick::getAxisPosition(m_joystickId,axisName)*JOYSTICK_SCALE;
    }

    static const sf::Joystick::Axis mapAxesNums[];

public:
    bool initInput(int joystickId = 0);
    bool updateState();
    bool releaseInput();

    explicit XInput() : m_f_haveJoystick(false)
    {

    }

    virtual ~XInput()
    {
        releaseInput();
    }

    bool haveJoystick() { return m_f_haveJoystick; }

    bool isKeyPressed(int key)
    {
        if (key < 32) return m_joyKeyState[key];
        return false;
    }

    // Normalized outputs are all FLOAT in range [-1.0f to 1.0f]
    float getLeftX()
    {
        return m_leftx;
    }

    float getLeftY()
    {
        return m_lefty;
    }

    float getRightY()
    {
        return m_righty;
    }

    float getRightX()
    {
        return m_rightx;
    }

    float getLeftThrottle()
    {
        return m_leftthrottle;
    }

    float getRightThrottle()
    {
        return m_rightthrottle;
    }

    float getS1()
    {
        return m_s1;
    }

    float getS2()
    {
        return m_s2;
    }

    int getRawAxisCount();

    float getRawAxis(int axis_id)
    {
        if (axis_id < getRawAxisCount())
            return this->getScaledAxis(mapAxesNums[axis_id]);
        else
            return 0.0f;
    }
};

#endif // XINPUTGAMEPAD_H
