#include "backend.h"


#include <QDebug>

#include "serial/serialhandler.h"
#define Serial SerialHandler::instance()


Backend::Backend(QObject *view) :
    m_inputs(new ControllerHandler),
    m_updater(new MiniBoardUpdater),
    m_threadarray(new ThreadArray),
    m_view(view)
{
    m_threadarray->push(Serial, false);
    m_threadarray->push(m_inputs, false);
    m_threadarray->push(m_updater, false);

    Serial->connectDevice();



    connect(Serial->p(), SIGNAL(batteryVoltageReceived(quint16)), this, SLOT(setUIVoltage(quint16)));
    connect(Serial->p(), SIGNAL(driveMotorPowerReceived(qint8, qint8, qint8, qint8, qint8, qint8)), this, SLOT(setUIDriveMotorPower(qint8,qint8,qint8,qint8,qint8,qint8)));
    connect(Serial->p(), SIGNAL(swerveDriveStateReceived(quint8)), this, SLOT(setUIDriveState(quint8)));
    connect(Serial->p(), SIGNAL(armMotorsReceived(qint8, qint8, qint8, qint8, qint8)), this, SLOT(setUIArmMotor(qint8, qint8, qint8, qint8, qint8)));
    connect(Serial->p(), SIGNAL(selectCameraReceived(quint8)), this, SLOT(setUICameraSelected(quint8)));
    connect(Serial->p(), SIGNAL(potentiometersReceived(quint8, quint8, quint8, quint8, quint8)), this, SLOT(setUIPotentiometers(quint8,quint8,quint8,quint8,quint8)));
    connect(Serial->p(), SIGNAL(callsignReceived(QByteArray)), this, SLOT(setUICallSign(QByteArray)));
    connect(Serial->p(), SIGNAL(magnetometerReceived(qint16, qint16, qint16)), this, SLOT(setUIMagnetometer(qint16,qint16,qint16)));
    connect(Serial->p(), SIGNAL(gyroscopeReceived(qint16, qint16, qint16)), this, SLOT(setUIGyroscope(qint16,qint16,qint16)));
    connect(Serial->p(), SIGNAL(gpioDirectionReceived(quint8)), this, SLOT(setUIGpioDirection(quint8)));
    connect(Serial->p(), SIGNAL(gpioOutValueReceived(quint8)), this, SLOT(setUIGpioOut(quint8)));
    connect(Serial->p(), SIGNAL(gpioReadStateReceived(quint8)), this, SLOT(setUIGpioReadState(quint8)));
    connect(Serial->p(), SIGNAL(debuggingInfoReceived(QByteArray)), this, SLOT(setUIDebugInfo(QByteArray)));
    connect(Serial->p(), SIGNAL(buildInfoReceived(QByteArray)), this, SLOT(setUIBuildInfo(QByteArray)));
    connect(Serial->p(), SIGNAL(gpsPositionReceived(quint8, qint64, qint64, qint32)), this, SLOT(setUIGpsPosition(quint8, qint64, qint64, qint32)));
    connect(Serial->p(), SIGNAL(gpsTrackReceived(quint8 , qint16 , quint16)), this, SLOT(setUIGpsTrack(quint8,qint16,quint16)));


    connect(m_view, SIGNAL(serialHandlerOn()), Serial, SLOT(start()));
    connect(m_view, SIGNAL(serialHandlerOff()), Serial, SLOT(stop()));
    connect(Serial, SIGNAL(changeButtonColor(QString, bool)), this, SLOT(colorSerialHandler(QString, bool)));

    connect(m_view, SIGNAL(updaterOn()), m_updater, SLOT(start()));
    connect(m_view, SIGNAL(updaterOff()), m_updater, SLOT(stop()));
    connect(m_updater, SIGNAL(changeButtonColor(QString,bool)), this, SLOT(colorUpdater(QString, bool)));

    connect(m_view, SIGNAL(controllerHandlerOn()), m_inputs, SLOT(start()));
    connect(m_view, SIGNAL(controllerHandlerOff()), m_inputs, SLOT(stop()));
    connect(m_inputs, SIGNAL(changeButtonColor(QString,bool)), this, SLOT(colorControllerHandler(QString, bool)));

    connect(m_view, SIGNAL(pauseAllThreads()), this, SLOT(pauseThreads()));
    connect(m_view, SIGNAL(resumeAllThreads()), this, SLOT(resumeThreads()));
    connect(m_view, SIGNAL(allThreadsClose()), this, SLOT(close()));
    connect(m_view, SIGNAL(closing(QQuickCloseEvent)), this, SLOT(close()));

    if (m_view)
        m_view->setProperty("battery_voltage", 2017);
}

//would need to destruct in the close button as well
Backend::~Backend()
{
    close();
}


/* *
 * Might need to add an "exec" to this,
 * as well as a "_connectedAlready" bool
 */


//add exec




void Backend::close()
{

    qDebug() << "start close";
    if (!m_closing){
        m_closing = true;
        m_threadarray->clear();

        delete m_threadarray;

        qDebug() << "closed";

        m_view->deleteLater();
    }
}

void Backend::pauseThreads(){
    Serial->stop();
    m_inputs->stop();
    m_updater->stop();
}

void Backend::resumeThreads(){
    Serial->start();
    m_inputs->start();
    m_updater->start();
}

void Backend::setUIVoltage(quint16 battery_voltage){
    //QObject *rect = m_view->findChild<QObject*>("window");
    if (m_view)
        m_view->setProperty("battery_voltage", battery_voltage);
}
void Backend::setUIDriveMotorPower(qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive){
    if (m_view){
        m_view->setProperty("l_f_drive", l_f_drive);
        m_view->setProperty("l_m_drive", l_m_drive);
        m_view->setProperty("l_b_drive", l_b_drive);
        m_view->setProperty("r_f_drive", r_f_drive);
        m_view->setProperty("r_m_drive", r_m_drive);
        m_view->setProperty("r_b_drive", r_b_drive);
    }
}
void Backend::setUIDriveState(quint8 swerve_state){
    if (m_view){
        m_view->setProperty("swerve_state", swerve_state);
    }
}
void Backend::setUIArmMotor(qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5){
    if (m_view){
        m_view->setProperty("arm_motor_1", arm_motor_1);
        m_view->setProperty("arm_motor_2", arm_motor_2);
        m_view->setProperty("arm_motor_3", arm_motor_3);
        m_view->setProperty("arm_motor_4", arm_motor_4);
        m_view->setProperty("arm_motor_5", arm_motor_5);
    }
}
void Backend::setUICameraSelected(quint8 selected_camera){
    if (m_view){
        m_view->setProperty("selected_camera", selected_camera);
    }
}
void Backend::setUIPotentiometers(quint8 pot_1, quint8 pot_2, quint8 pot_3, quint8 pot_4, quint8 pot_5){
    if (m_view){
        m_view->setProperty("pot_1", pot_1);
        m_view->setProperty("pot_2", pot_2);
        m_view->setProperty("pot_3", pot_3);
        m_view->setProperty("pot_4", pot_4);
        m_view->setProperty("pot_5", pot_5);
    }
}
void Backend::setUICallSign(QByteArray callsign_data){
    if (m_view){
        m_view->setProperty("callsign_data", callsign_data);
    }
}
void Backend::setUIMagnetometer(qint16 mag_x, qint16 mag_y, qint16 mag_z){
    if (m_view){
        m_view->setProperty("mag_x", mag_x);
        m_view->setProperty("mag_y", mag_y);
        m_view->setProperty("mag_z", mag_z);
    }
}
void Backend::setUIGyroscope(qint16 gyro_x, qint16 gyro_y, qint16 gyro_z){
    if (m_view){
        m_view->setProperty("gyro_x", gyro_x);
        m_view->setProperty("gyro_y", gyro_y);
        m_view->setProperty("gyro_z", gyro_z);
    }
}
void Backend::setUIGpioDirection(quint8 gpio_dir){
    if (m_view){
        m_view->setProperty("gpio_dir", gpio_dir);
    }
}
void Backend::setUIGpioOut(quint8 gpio_out){
    if (m_view){
        m_view->setProperty("gpio_out", gpio_out);
    }
}
void Backend::setUIGpioReadState(quint8 gpio_state){
    if (m_view){
        m_view->setProperty("gpio_state", gpio_state);
    }
}

void Backend::setUIGpsPosition(quint8 gps_pos_valid, qint64 latitude, qint64 longitude, qint32 altitude)
{
    if(m_view && (gps_pos_valid != 0)){
        double lat = static_cast<double>(latitude) / (100000.0 * 60.0);
        double lon = static_cast<double>(longitude) / (100000.0 * 60.0);

        m_view->setProperty("latitude", lat);
        m_view->setProperty("longitude", lon);
    }
}

void Backend::setUIDebugInfo(QByteArray debug_str_data){
    if (m_view){
        m_view->setProperty("debug_str_data", debug_str_data);
    }
}
void Backend::setUIBuildInfo(QByteArray build_info_data){
    if (m_view){
        m_view->setProperty("build_info_data", build_info_data);
    }
}
void Backend::setUIGpsPos(quint8 gps_pos_valid, qint64 latitude, qint64 longitude, qint32 altitude){
    if(m_view){
        m_view->setProperty("gps_pos_valid",gps_pos_valid);
        m_view->setProperty("latitude", latitude);
        m_view->setProperty("longitude", longitude);
        m_view->setProperty("altitude", altitude);
    }
}
void Backend::setUIGpsTrack(quint8 gps_track_valid, qint16 gps_heading, quint16 gps_speed){
    if (m_view){
        m_view->setProperty("gps_track_valid", gps_track_valid);
        m_view->setProperty("gps_heading", gps_heading);
        m_view->setProperty("gps_speed", gps_speed);
    }
}

void Backend::colorSerialHandler(QString color, bool isActive){
    if (m_view){
        m_view->setProperty("colorSerialSerial", color);
        m_view->setProperty("activeSeriaSerial", isActive);
    }
}
void Backend::colorControllerHandler(QString color, bool isActive){
    if (m_view){
        m_view->setProperty("colorControllerSerial", color);
        m_view->setProperty("activeControllerSerial", isActive);
    }
}
void Backend::colorUpdater(QString color, bool isActive){
    if (m_view){
        m_view->setProperty("colorUpdater", color);
        m_view->setProperty("activeUpdater", isActive);
    }
}
