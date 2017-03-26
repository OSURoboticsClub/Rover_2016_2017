#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector2D>
#include <QVector>
#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QQuickWidget>
#include <QThread>
#include <QQuickView>
#include <QTime>

#include "serial/serialhandler.h"
#include "inputs/controllerhandler.h"
#include "threading/threadarray.h"
#include "serial/miniboardupdater.h"

#define Handler SerialHandler::instance()->p()

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(QObject *);

    ~MainWindow();

private:

    QObject *item;

    ThreadArray *threadarray;

    QSerialPort output;

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

#endif // MAINWINDOW_H
