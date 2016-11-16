#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <QGamepad>

#include "abstractcontroller.h"

class XboxController : public AbstractController
{
    Q_OBJECT
public:
    XboxController();
    ~XboxController();

private:
    QGamepad *m_gamepad;
};

#endif // XBOXCONTROLLER_H
