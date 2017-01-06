#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickWidget>
#include <QThread>
#include <QTime>

#include "serial/serialhandler.h"
#include "inputs/controllerhandler.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    virtual void closeEvent ( QCloseEvent * event );
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSerialPort output;
    SerialHandler *serialRead;
    bool _serialRunning;
    int numThreads;
    QThread **threadArray;

    SerialHandler *m_serial;
    ControllerHandler *m_controller;

public slots:
    void connectSerial();

private slots:

signals:
    void startReadIn();
    void stopReadIn();
    void closeThreads();


};

#endif // MAINWINDOW_H
