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

    connect(this, SIGNAL(startSerial()), SerialHandler::instance(), SLOT(stop()));
    connect(this, SIGNAL(stopSerial()), SerialHandler::instance(), SLOT(stop()));
    connect(this, SIGNAL(startInputs()), m_inputs, SLOT(start()));
    connect(this, SIGNAL(stopInputs()), m_inputs, SLOT(stop()));

    connect(Handler, SIGNAL(batteryVoltageReceived(quint16)), this, SLOT(setUIVoltage(quint16)));
    connect(Handler, SIGNAL(driveMotorPowerReceived(qint8, qint8, qint8, qint8, qint8, qint8)), this, SLOT(setUIDriveMotorPower(qint8,qint8,qint8,qint8,qint8,qint8)));
    connect(Handler, SIGNAL(swerveDriveStateReceived(quint8)), this, SLOT(setUIDriveState(quint8)));
    connect(Handler, SIGNAL(armMotorsReceived(qint8, qint8, qint8, qint8, qint8)), this, SLOT(setUIArmMotor(qint8, qint8, qint8, qint8, qint8)));
    connect(Handler, SIGNAL(selectCameraReceived(quint8)), this, SLOT(setUICameraSelected(quint8)));
    connect(Handler, SIGNAL(potentiometersReceived(quint8, quint8, quint8, quint8, quint8)), this, SLOT(setUIPotentiometers(quint8,quint8,quint8,quint8,quint8)));
    connect(Handler, SIGNAL(callsignReceived(QByteArray)), this, SLOT(setUICallSign(QByteArray)));
    connect(Handler, SIGNAL(magnetometerReceived(qint16, qint16, qint16)), this, SLOT(setUIMagnetometer(qint16,qint16,qint16)));
    connect(Handler, SIGNAL(gyroscopeReceived(qint16, qint16, qint16)), this, SLOT(setUIGyroscope(qint16,qint16,qint16)));
    connect(Handler, SIGNAL(gpioDirectionReceived(quint8)), this, SLOT(setUIGpioDirection(quint8)));
    item = _item;

    connect(item, SIGNAL(_serialHandlerOn()), SerialHandler::instance(), SLOT(start()));
    connect(item, SIGNAL(_serialHandlerOff()), SerialHandler::instance(), SLOT(stop()));

    connect(item, SIGNAL(_updaterOn()), m_updater, SLOT(start()));
    connect(item, SIGNAL(_updaterOff()), m_updater, SLOT(stop()));

    connect(item, SIGNAL(_controllerHandlerOn()), m_inputs, SLOT(start()));
    connect(item, SIGNAL(_controllerHandlerOff()), m_inputs, SLOT(stop()));

    connect(item, SIGNAL(_allThreadsClose()), this, SLOT(close()));


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
void MainWindow::setUIDriveState(quint8 swerve_state){
    if (item){
        item->setProperty("swerve_state", swerve_state);
    }
}
void MainWindow::setUIArmMotor(qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5){
    if (item){
        item->setProperty("arm_motor_1", arm_motor_1);
        item->setProperty("arm_motor_2", arm_motor_2);
        item->setProperty("arm_motor_3", arm_motor_3);
        item->setProperty("arm_motor_4", arm_motor_4);
        item->setProperty("arm_motor_5", arm_motor_5);
    }
}
void MainWindow::setUICameraSelected(quint8 selected_camera){
    if (item){
        item->setProperty("selected_camera", selected_camera);
    }
}
void MainWindow::setUIPotentiometers(quint8 pot_1, quint8 pot_2, quint8 pot_3, quint8 pot_4, quint8 pot_5){
    if (item){
        item->setProperty("pot_1", pot_1);
        item->setProperty("pot_2", pot_2);
        item->setProperty("pot_3", pot_3);
        item->setProperty("pot_4", pot_4);
        item->setProperty("pot_5", pot_5);
    }
}
void MainWindow::setUICallSign(QByteArray callsign_data){
    if (item){
        item->setProperty("callsign_data", callsign_data);
    }
}
void MainWindow::setUIMagnetometer(qint16 mag_x, qint16 mag_y, qint16 mag_z){
    if (item){
        item->setProperty("mag_x", mag_x);
        item->setProperty("mag_y", mag_y);
        item->setProperty("mag_z", mag_z);
    }
}
void MainWindow::setUIGyroscope(qint16 gyro_x, qint16 gyro_y, qint16 gyro_z){
    if (item){
        item->setProperty("gyro_x", gyro_x);
        item->setProperty("gyro_y", gyro_y);
        item->setProperty("gyro_z", gyro_z);
    }
}
void MainWindow::setUIGpioDirection(quint8 gpio_dir){
    if (item){
        item->setProperty("gpio_dir", gpio_dir);
    }
}
