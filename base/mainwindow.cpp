#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "io.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSerial()
{

    serial.setPortName("pts/11");

    if(serial.open(QIODevice::ReadWrite)){
        if(!serial.setBaudRate(QSerialPort::Baud57600))
            qDebug()<<serial.errorString();

        if(!serial.setDataBits(QSerialPort::Data8))
            qDebug()<<serial.errorString();

        if(!serial.setParity(QSerialPort::NoParity))
            qDebug()<<serial.errorString();

        if(!serial.setStopBits(QSerialPort::OneStop))
            qDebug()<<serial.errorString();

        if(!serial.setFlowControl(QSerialPort::NoFlowControl))
            qDebug()<<serial.errorString();
    }

    else {
        qDebug() << serial.errorString();
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->connectSerial();
}

void MainWindow::on_pushButton_2_clicked()
{

    packets_BatteryVoltage message = {22};
    uint8_t *output = encodePacket(&message, BATTERYVOLTAGE_TYPE);
    if(*output == BATTERYVOLTAGE_TYPE)
    {
        packets_BatteryVoltage p = decodeBatteryVoltage(output, *output);
        qDebug() << p.battery_voltage;
    }
    free(output);

    packets_MotorPower message2 = {5, 6};
    uint8_t *output2 = encodePacket(&message2, MOTORPOWER_TYPE);
    if(*output2 == MOTORPOWER_TYPE)
    {
        packets_MotorPower p2 = decodeMotorPower(output2, *output2);
        qDebug() << p2.left_motor;
        qDebug() << p2.right_motor;
    }
    free(output2);
}
