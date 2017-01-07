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

    bool _serialRunning;
    int numThreads;
    QThread **threadArray;

    ControllerHandler *m_inputs;

public slots:
    void connectSerial();

private slots:

    void on_actionStart_Thread_triggered();

    void on_actionStop_Thread_triggered();

    void on_actionStart_Thread_2_triggered();

    void on_actionStop_Thread_2_triggered();

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
