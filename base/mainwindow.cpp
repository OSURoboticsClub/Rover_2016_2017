#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QBuffer>


MainWindow::MainWindow(QObject *_item) :
    m_inputs(new ControllerHandler),
    m_updater(new MiniBoardUpdater)
{
    threadarray = new ThreadArray();
    threadarray->push(SerialHandler::instance(), false);
    threadarray->push(m_inputs, false);
    threadarray->push(m_updater, false);

    SerialHandler::instance()->connectDevice();



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
    connect(Handler, SIGNAL(gpioOutValueReceived(quint8)), this, SLOT(setUIGpioOut(quint8)));
    connect(Handler, SIGNAL(gpioReadStateReceived(quint8)), this, SLOT(setUIGpioReadState(quint8)));
    connect(Handler, SIGNAL(debuggingInfoReceived(QByteArray)), this, SLOT(setUIDebugInfo(QByteArray)));
    connect(Handler, SIGNAL(buildInfoReceived(QByteArray)), this, SLOT(setUIBuildInfo(QByteArray)));
    connect(Handler, SIGNAL(gpsPositionReceived(quint8, qint64, qint64, qint32)), this, SLOT(setUIGpsPosition(quint8, qint64, qint64, qint32)));
    connect(Handler, SIGNAL(gpsTrackReceived(quint8 , qint16 , quint16)), this, SLOT(setUIGpsTrack(quint8,qint16,quint16)));

    item = _item;

    //connect(item, SIGNAL(close), )

    connect(item, SIGNAL(_serialHandlerOn()), SerialHandler::instance(), SLOT(start()));
    connect(item, SIGNAL(_serialHandlerOff()), SerialHandler::instance(), SLOT(stop()));
    connect(SerialHandler::instance(), SIGNAL(changeButtonColor(QString, bool)), this, SLOT(colorSerialHandler(QString, bool)));

    connect(item, SIGNAL(_updaterOn()), m_updater, SLOT(start()));
    connect(item, SIGNAL(_updaterOff()), m_updater, SLOT(stop()));
    connect(m_updater, SIGNAL(changeButtonColor(QString,bool)), this, SLOT(colorUpdater(QString, bool)));

    connect(item, SIGNAL(_controllerHandlerOn()), m_inputs, SLOT(start()));
    connect(item, SIGNAL(_controllerHandlerOff()), m_inputs, SLOT(stop()));
    connect(m_inputs, SIGNAL(changeButtonColor(QString,bool)), this, SLOT(colorControllerHandler(QString, bool)));

    //connect(item, SIGNAL(close()), this, SLOT(close()));
    connect(item, SIGNAL(_pauseAllThreads()), this, SLOT(pauseThreads()));
    connect(item, SIGNAL(_resumeAllThreads()), this, SLOT(resumeThreads()));
    connect(item, SIGNAL(_allThreadsClose()), this, SLOT(close()));
    connect(item, SIGNAL(closing(QQuickCloseEvent)), this, SLOT(close()));

    connect(item, SIGNAL(_connectDevice()), SerialHandler::instance(), SLOT(connectDevice()));
}

//would need to destruct in the close button as well
MainWindow::~MainWindow()
{
    close();
}


/* *
 * Might need to add an "exec" to this,
 * as well as a "_connectedAlready" bool
 */


//add exec




void MainWindow::close()
{

    qDebug() << "start close";
    if (!m_closing){
        m_closing = true;
        threadarray->clear();

        delete threadarray;

        qDebug() << "closed";

        item->deleteLater();
    }
}


void MainWindow::pauseThreads(){
    SerialHandler::instance()->stop();
    m_inputs->stop();
    m_updater->stop();
}
void MainWindow::resumeThreads(){
    SerialHandler::instance()->start();
    m_inputs->start();
    m_updater->start();
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
void MainWindow::setUIGpioOut(quint8 gpio_out){
    if (item){
        item->setProperty("gpio_out", gpio_out);
    }
}
void MainWindow::setUIGpioReadState(quint8 gpio_state){
    if (item){
        item->setProperty("gpio_state", gpio_state);
    }
}

void MainWindow::setUIGpsPosition(quint8 gps_pos_valid, qint64 latitude, qint64 longitude, qint32 altitude)
{
    if(item && (gps_pos_valid != 0)){
        double lat = static_cast<double>(latitude) / (100000.0 * 60.0);
        double lon = static_cast<double>(longitude) / (100000.0 * 60.0);

        item->setProperty("latitude", lat);
        item->setProperty("longitude", lon);
    }
}

void MainWindow::setUIDebugInfo(QByteArray debug_str_data){
    if (item){
        item->setProperty("debug_str_data", debug_str_data);
    }
}
void MainWindow::setUIBuildInfo(QByteArray build_info_data){
    if (item){
        item->setProperty("build_info_data", build_info_data);
    }
}
void MainWindow::setUIGpsPos(quint8 gps_pos_valid, qint64 latitude, qint64 longitude, qint32 altitude){
    if(item){
        item->setProperty("gps_pos_valid",gps_pos_valid);
        item->setProperty("latitude", latitude);
        item->setProperty("longitude", longitude);
        item->setProperty("altitude", altitude);
    }
}
void MainWindow::setUIGpsTrack(quint8 gps_track_valid, qint16 gps_heading, quint16 gps_speed){
    if (item){
        item->setProperty("gps_track_valid", gps_track_valid);
        item->setProperty("gps_heading", gps_heading);
        item->setProperty("gps_speed", gps_speed);
    }
}

void MainWindow::colorSerialHandler(QString color, bool activeSeriaHandler){
    if (item){
        item->setProperty("colorSerialHandler", color);
        item->setProperty("activeSeriaHandler", activeSeriaHandler);
    }
}
void MainWindow::colorControllerHandler(QString color, bool activeControllerHandler){
    if (item){
        item->setProperty("colorControllerHandler", color);
        item->setProperty("activeControllerHandler", activeControllerHandler);
        if (m_inputs->frSky != '\0'){
            connect(m_inputs->frSky, SIGNAL(frSkyPaused(qint16)), this, SLOT(frSkyPaused(qint16)));
            connect(m_inputs->frSky, SIGNAL(frSkyModeChange(qint16)), this, SLOT(frSkyModeChange(qint16)));
        }
    }
}
void MainWindow::colorUpdater(QString color, bool activeUpdater){
    if (item){
        item->setProperty("colorUpdater", color);
        item->setProperty("activeUpdater", activeUpdater);
    }
}
void MainWindow::frSkyPaused(qint16 _frSkyPaused){
    if (item){
        item->setProperty("frSkyPaused", _frSkyPaused);
    }
}
void MainWindow::frSkyModeChange(qint16 _frSkyModeChanged){
    if (item){
        item->setProperty("frSkyModeChanged", _frSkyModeChanged);
    }
}
