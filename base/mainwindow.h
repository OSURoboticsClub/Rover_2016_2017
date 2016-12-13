#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QQuickWidget>
#include <QThread>

#include "serialhandler.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort output;
    SerialHandler *serialRead;
    bool _serialRunning;

public slots:
    void connectSerial();

private slots:
    void on_pushButton_4_clicked();
    void on_serialRead_clicked();
    void on_exit_clicked();

signals:
    void startReadIn();
};

#endif // MAINWINDOW_H
