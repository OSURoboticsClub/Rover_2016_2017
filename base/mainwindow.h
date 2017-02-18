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
#include "threadarray.h"
#include "miniboardupdater.h"

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
    void setUIVoltage(quint16);
    void setUIDriveMotorPower(qint8, qint8, qint8, qint8, qint8, qint8);
    //batteryVoltageReceived(battery_voltage)

private slots:

    void on_actionStart_Thread_triggered();

    void on_actionStop_Thread_triggered();

    void on_actionStart_Thread_2_triggered();

    void on_actionStop_Thread_2_triggered();

    void on_actionPing_triggered();

    void on_actionAutodetect_Serial_triggered();

    void on_actionIdentify_controllers_triggered();

signals:

    void startSerial();
    void stopSerial();
    void startInputs();
    void stopInputs();

    void startReadIn();
    void stopReadIn();
    void closeThreads();
    void startThreads();


};

#endif // MAINWINDOW_H
