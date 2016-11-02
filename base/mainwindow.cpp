#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serial = new SerialHandler();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectSerial()
{

    output.setPortName("pts/9");

    if(output.open(QIODevice::WriteOnly)){
        if(!output.setBaudRate(QSerialPort::Baud57600))
            qDebug()<<output.errorString();

        if(!output.setDataBits(QSerialPort::Data8))
            qDebug()<<output.errorString();

        if(!output.setParity(QSerialPort::NoParity))
            qDebug()<<output.errorString();

        if(!output.setStopBits(QSerialPort::OneStop))
            qDebug()<<output.errorString();

        if(!output.setFlowControl(QSerialPort::NoFlowControl))
            qDebug()<<output.errorString();
    }

    else {
        qDebug() << output.errorString();
    }
}


void MainWindow::on_pushButton_clicked()
{
    this->connectSerial();
}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{
    this->m_serial->setupPort("/dev/pts/9");
    this->m_serial->run();
}
