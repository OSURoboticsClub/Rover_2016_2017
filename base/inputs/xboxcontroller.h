#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H


#include "abstractcontroller.h"

class XboxController : public AbstractController
{
    Q_OBJECT
public:
    explicit XboxController(QFile *file, QObject *parent = 0);
    ~XboxController();

signals:

protected:
    void emitAxisChanges(quint8 axisIndex) Q_DECL_OVERRIDE;
    void emitButtonChanges(quint8 buttonIndex) Q_DECL_OVERRIDE;
};

#endif // XBOXCONTROLLER_H
