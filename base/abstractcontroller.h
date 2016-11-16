#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = 0);

signals:

public slots:
};

#endif // ABSTRACTCONTROLLER_H
