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
    numThreads = 1;
    //QThread **threadArray = new QThread *[numThreads]; //1 thread
    //threadArray[0] = serialRead;
    serialRead->start();

    for (int i = 0; i < numThreads;i++)
    {
        //threadArray[i]->start();
    }

    connect(this, SIGNAL(closeThreads()), serialRead, SLOT(stopThread()));
    _serialRunning = false;
}

//would need to destruct in the close button as well
MainWindow::~MainWindow()
{
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


void MainWindow::closeEvent(QCloseEvent *event)
{

    qDebug() << "start close";
    emit closeThreads();
    serialRead->wait();
    /*
    for (int i = 0; i < numThreads; i++)
    {
        delete [] threadArray[i];
    }

    delete [] threadArray;
    */
    delete serialRead;
    delete ui;
    qDebug() << "end close";
    event->accept();


    /*emit closeThreads();
    bool allThreadsKilled = false;
    int numThreadsRunning = 0;
    bool terminateHasRun = false;

    QTime startTime = currentTime();

    while (!allThreadsKilled)
    {
        if ()
    }*/

}

/*
 * destructor won't be called if you exit main thread,
 * so have to add destructor functionality to both
 */
void MainWindow::on_exit_clicked()
{
    this->close();
}
