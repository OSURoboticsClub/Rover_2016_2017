#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector2D>
#include <QVector>
#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QQuickWidget>
#include <QThread>
#include <QTime>

#include "serialhandler.h"
#include "inputs/controllerhandler.h"
#include "threadarray.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    virtual void closeEvent (QCloseEvent *event);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialHandler *m_serial;
    ControllerHandler *m_controller;
    bool _serialRunning;
    int numThreads;
    ThreadArray *threadarray;
    //QThread **threadArray;
    //QVector<QVector<QThread>> threadArray;

private slots:
    void on_exit_clicked();

signals:
    void closeThreads();
};

#endif // MAINWINDOW_H
