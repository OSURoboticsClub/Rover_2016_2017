#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QQuickWidget>
#include <QTimer>

#include "serialhandler.h"
#include "frsky.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SerialHandler *m_serial;

private:
    Ui::MainWindow *ui;
    QSerialPort output;
    FrSky frsky;
    QTimer tmr;

    bool f_haveJoystick;

public slots:
    void connectSerial();

private slots:
    void on_btnConnectSerials_clicked();
    void on_btnSendMsg_clicked();
    void on_btnReadInput_clicked();
};

#endif // MAINWINDOW_H
