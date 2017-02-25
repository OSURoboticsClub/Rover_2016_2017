#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H


#include "abstractcontroller.h"


class XboxController : public AbstractController
{
    Q_OBJECT
public:
    explicit XboxController(int id, QObject *parent = 0);
    ~XboxController();

signals:

protected:
    void emitAxisChanges(int axisIndex);
    void emitButtonChanges(int buttonIndex);
};

#endif // XBOXCONTROLLER_H
