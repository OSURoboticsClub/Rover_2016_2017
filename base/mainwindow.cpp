#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_inputs(new ControllerHandler)
{
    ui->setupUi(this);

    numThreads = 0;
    QThread *threadArray[numThreads];

    connect(this, SIGNAL(startSerial()), SerialHandler::instance(), SLOT(start()));
    connect(this, SIGNAL(stopSerial()), SerialHandler::instance(), SLOT(stop()));
    connect(this, SIGNAL(startInputs()), m_inputs, SLOT(start()));
    connect(this, SIGNAL(stopInputs()), m_inputs, SLOT(quit()));
    _serialRunning = false;
}

//would need to destruct in the close button as well
MainWindow::~MainWindow()
{
    this->close();
    delete ui;
    delete m_inputs;
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

/*
 * Part I: Close event without timeout check
 * Part II: Close event with Timeout check
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    /*
    qDebug() << "start close";
    emit closeThreads();
    serialRead->wait();

    for (int i = 0; i < numThreads; i++)
    {
        delete threadArray[i];
    }
    delete threadArray;

    //delete serialRead;
    delete ui;
    qDebug() << "end close";
    event->accept();
    */

    qDebug() << "closing";
    emit closeThreads();
    bool allThreadsKilled = false;
    int numThreadsRunning = 0;
    bool terminateHasRun = false;

    QTime startTime;
    startTime.start();

    while (!allThreadsKilled)
    {
        for (int i = 0;i < numThreads;i++)
        {
            if (threadArray[i]->isRunning())
            {
                numThreadsRunning++;
            }
        }
        if (numThreadsRunning > 0)
        {
            numThreadsRunning = 0;

            if (!terminateHasRun)
            {
                if (startTime.elapsed() > 8000) //terminate time is 8 seconds. Change to appropriate amount later
                {
                    for (int k = 0;k < numThreads;k++)
                    {
                        if (threadArray[k]->isRunning())
                        {
                            qDebug() << "Terminating thread: " + k;
                            threadArray[k]->terminate();
                        }
                    }
                    terminateHasRun = true;
                    allThreadsKilled = true;
                }
            }
        }

        else
        {
            allThreadsKilled = true;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        delete threadArray[i];
    }
    delete threadArray;
    delete ui;

    qDebug() << "closed";
    event->accept();
}

/*
 * destructor won't be called if you exit main thread,
 * so have to add destructor functionality to both
 */

void MainWindow::on_actionStart_Thread_triggered()
{
    emit startSerial();
}

void MainWindow::on_actionStop_Thread_triggered()
{
    emit stopSerial();
}

void MainWindow::on_actionStart_Thread_2_triggered()
{
    emit startInputs();
}

void MainWindow::on_actionStop_Thread_2_triggered()
{
    emit stopInputs();
}
