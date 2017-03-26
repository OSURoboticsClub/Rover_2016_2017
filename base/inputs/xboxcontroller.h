#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <QFile>

#include "abstractcontroller.h"


class XboxController : public AbstractController
{
    Q_OBJECT
public:
    explicit XboxController(QFile *file, QObject *parent = 0);
    ~XboxController();

signals:

protected:
    void emitAxisChanges(int axisIndex);
    void emitButtonChanges(int buttonIndex);
};

#endif // XBOXCONTROLLER_H
