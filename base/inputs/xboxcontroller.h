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
    void emitAxisChanges(int axisIndex) Q_DECL_OVERRIDE;
    void emitButtonChanges(int buttonIndex) Q_DECL_OVERRIDE;
};

#endif // XBOXCONTROLLER_H
