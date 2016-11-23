#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QQuickWidget>

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
    SerialHandler *m_serial;

private:
    Ui::MainWindow *ui;
    QSerialPort output;
    SerialHandler *serialRead;

public slots:
    void connectSerial();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_serialRead_clicked();

signals:
 void serialRead_start();
};

#endif // MAINWINDOW_H
