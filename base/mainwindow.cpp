#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "commgen.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    threadarray = new ThreadArray();
    m_serial = new SerialHandler();
    m_controller = new ControllerHandler();
    /*
    numThreads = 2;
    threadArray = new QThread *[numThreads];
    //QVector<QVector<QThread>> threadArray(numThreads);
    threadArray[0] = m_serial;
    threadArray[1] = m_controller;
    //threadArray.push_back(m_serial);
    //threadArray.push_back(m_controller);
    for (int i = 0; i < numThreads;i++)
    {
        threadArray[i]->start();
    }

    connect(this, SIGNAL(closeThreads()), m_serial, SLOT(stop()));
    connect(this,SIGNAL(closeThreads()), m_controller, SLOT(stop()));
    */


    threadarray->push(m_serial);
    threadarray->push(m_controller);
    _serialRunning = false;
}

MainWindow::~MainWindow()
{

}



void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    qDebug() << "start close";
    /*
    emit closeThreads();
    m_serial->wait();
    for (int i = 0; i < numThreads; i++)
    {
        delete threadArray[i];
    }
    delete threadArray;
    //delete m_serial;
    delete ui;
    qDebug() << "end close";
    event->accept();


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
    */

    while (!threadarray->clear()){}
    delete threadarray;
    delete ui;

    qDebug() << "closed";
    event->accept();
}
