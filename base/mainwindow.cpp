#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "commgen.h"
#include "comm.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialRead = new SerialHandler();
    connect(this, SIGNAL(startReadIn()), serialRead, SLOT(readData()));
    connect(this, SIGNAL(stopReadIn()), serialRead, SLOT(stop()));
    //this will allow the serialRead thread to exit correctly once main window needs to close
    connect(serialRead, SIGNAL(finished()), serialRead, SLOT(deleteLater()));
    _serialRunning = false;
}

//would need to destruct in the close button as well
MainWindow::~MainWindow()
{
    qDebug() << "start destruct";
    serialRead->exit();
    delete ui;
    delete serialRead;
    qDebug() << "end destruct";
}


/* *
 * Might need to add an "exec" to this,
 * as well as a "_connectedAlready" bool
 */
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

//add exec
void MainWindow::on_pushButton_4_clicked()
{
    uint8_t buffer[10] = {};

}


/*
 * starts the serial read.
 * Port and all that junk needs to be handled in this thread/object
 */
void MainWindow::on_serialRead_clicked()
{
    if (!_serialRunning){
        serialRead->start();
        emit startReadIn();
        _serialRunning = true;

    }
}


/*
 * destructor won't be called if you exit main thread,
 * so have to add destructor functionality to both
 */
void MainWindow::on_exit_clicked()
{
    if (serialRead != 0 && serialRead->isRunning() ) {
        serialRead->requestInterruption();
        serialRead->wait();
    }

    exit(0);
}
