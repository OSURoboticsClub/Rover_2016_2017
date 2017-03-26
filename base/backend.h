#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include "inputs/controllerhandler.h"
#include "threading/threadarray.h"
#include "serial/miniboardupdater.h"


class Backend : public QObject
{
    Q_OBJECT

public:
    Backend(QObject *);

    ~Backend();

private:

    QObject *item;

    ThreadArray *threadarray;

    bool m_closing = false;

    ControllerHandler *m_inputs;
    MiniBoardUpdater *m_updater;

public slots:
    void close();
    void pauseThreads();
    void resumeThreads();

    void setUIVoltage(quint16);
    void setUIDriveMotorPower(qint8, qint8, qint8, qint8, qint8, qint8);
    void setUIDriveState(quint8);
    void setUIArmMotor(qint8, qint8, qint8, qint8, qint8);
    void setUICameraSelected(quint8);
    void setUIPotentiometers(quint8, quint8, quint8, quint8, quint8);
    void setUICallSign(QByteArray);
    void setUIMagnetometer(qint16, qint16, qint16);
    void setUIGyroscope(qint16, qint16, qint16);
    void setUIGpioDirection(quint8);
    void setUIGpioOut(quint8);
    void setUIGpioReadState(quint8);
    void setUIGpsPosition(quint8, qint64, qint64, qint32);
    void setUIDebugInfo(QByteArray);
    void setUIBuildInfo(QByteArray);
    void setUIGpsPos (quint8,qint64,qint64,qint32);
    void setUIGpsTrack(quint8 , qint16 , quint16);

    void colorSerialHandler(QString, bool);
    void colorControllerHandler(QString, bool);
    void colorUpdater(QString, bool);
    //batteryVoltageReceived(battery_voltage)

private slots:


signals:


};

#endif // Backend_H
