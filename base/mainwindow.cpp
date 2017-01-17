#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QBuffer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_inputs(new ControllerHandler),
    m_serial(new SerialHandler)
{
    ui->setupUi(this);

    connect(this, SIGNAL(startSerial()), m_serial, SLOT(start()));
    connect(this, SIGNAL(stopSerial()), m_serial, SLOT(stop()));
    connect(this, SIGNAL(startInputs()), m_inputs, SLOT(start()));
    connect(this, SIGNAL(stopInputs()), m_inputs, SLOT(stop()));

    threadarray.push(m_serial, true);
    threadarray.push(m_inputs, true);

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


//add exec


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

void MainWindow::on_actionPing_triggered()
{


    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    SerialHandler::instance()->p()->setDevice(&buffer);

    SerialHandler::instance()->p()->writeCameraCommand(QByteArray("something"));

    qDebug() << buffer.data().toHex();



}

void MainWindow::on_actionAutodetect_Serial_triggered()
{
    SerialHandler::instance()->connectDevice();
}

void MainWindow::on_actionIdentify_controllers_triggered()
{
    m_inputs->resetControllers();

}


void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    qDebug() << "start close";

    threadarray.clear();
    delete ui;

    qDebug() << "closed";
    event->accept();
}
