#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QBuffer>


MainWindow::MainWindow(QObject *_item) :
    m_inputs(new ControllerHandler),
    m_updater(new MiniBoardUpdater)
{
    threadarray = new ThreadArray;
    threadarray->push(SerialHandler::instance(), false);
    threadarray->push(m_inputs, false);
    threadarray->push(m_updater, false);

    connect(this, SIGNAL(startSerial()), SerialHandler::instance(), SLOT(start()));
    connect(this, SIGNAL(stopSerial()), SerialHandler::instance(), SLOT(stop()));
    connect(this, SIGNAL(startInputs()), m_inputs, SLOT(start()));
    connect(this, SIGNAL(stopInputs()), m_inputs, SLOT(stop()));

    connect(Handler, SIGNAL(batteryVoltageReceived(quint16)), this, SLOT(setUIVoltage(quint16)));
    connect(Handler, SIGNAL(driveMotorPowerReceived(qint8, qint8, qint8, qint8, qint8, qint8)), this, SLOT(setUIDriveMotorPower(qint8,qint8,qint8,qint8,qint8,qint8)));

    item = _item;

    connect(item, SIGNAL(_serialHandlerOn()), SerialHandler::instance(), SLOT(start()));



    if (item)
        item->setProperty("battery_voltage", 2017);
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


    //QBuffer buffer;
    //buffer.open(QIODevice::ReadWrite);
    //SerialHandler::instance()->p()->setDevice(&buffer);

    SerialHandler::instance()->p()->readBatteryVoltage();



}

void MainWindow::on_actionAutodetect_Serial_triggered()
{
    SerialHandler::instance()->connectDevice();
}

void MainWindow::on_actionIdentify_controllers_triggered()
{
    m_inputs->resetControllers();

}


void MainWindow::close()
{

    qDebug() << "start close";
    if (!m_closing){
        m_closing = true;
        threadarray->clear();

        delete threadarray;

        qDebug() << "closed";
    }
}

void MainWindow::setUIVoltage(quint16 battery_voltage){
    //QObject *rect = item->findChild<QObject*>("window");
    if (item)
        item->setProperty("battery_voltage", battery_voltage);
}
void MainWindow::setUIDriveMotorPower(qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive){
    if (item){
        item->setProperty("l_f_drive", l_f_drive);
        item->setProperty("l_m_drive", l_m_drive);
        item->setProperty("l_b_drive", l_b_drive);
        item->setProperty("r_f_drive", r_f_drive);
        item->setProperty("r_m_drive", r_m_drive);
        item->setProperty("r_b_drive", r_b_drive);
    }
}
