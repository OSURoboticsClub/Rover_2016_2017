#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>


/* TODO: fill in this abstract class with virtual methods that correspond to
 * generic controls for a game controller.
 * https://github.com/OregonStateMarsRover/2011
 */
class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = 0);

signals:

public slots:
};

#endif // ABSTRACTCONTROLLER_H
