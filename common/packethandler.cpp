
#include "packethandler.h"

PacketHandler::PacketHandler(QObject *parent) :
    QObject(parent),
    m_datastream()
{
}

PacketHandler::PacketHandler(QIODevice *d, QObject *parent) :
    QObject(parent),
    m_datastream(d)
{
}

PacketHandler::PacketHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent) :
    QObject(parent),
    m_datastream(a, flags)
{
}

PacketHandler::~PacketHandler()
{
}

/* start and end bytes, as well as escape bytes removed */
void PacketHandler::parsePacket(quint8 size)
{
    /* datastream head at CRC */
    /* @parse_packets */
    quint16 _crc;
    m_datastream >> _crc;
    quint8 _packetType;
    m_datastream >> _packetType;
    if(_packetType == static_cast<quint8>(Types::PAUSE)) {
        quint8 pause_state;
        m_datastream >> pause_state;
        emit pauseReceived(pause_state);
    }
    else if(_packetType == static_cast<quint8>(Types::BATTERY_VOLTAGE)) {
        quint16 battery_voltage;
        m_datastream >> battery_voltage;
        emit batteryVoltageReceived(battery_voltage);
    }
    else if(_packetType == static_cast<quint8>(Types::DRIVE_MOTOR_POWER)) {
        qint8 l_f_drive;
        m_datastream >> l_f_drive;
        qint8 l_m_drive;
        m_datastream >> l_m_drive;
        qint8 l_b_drive;
        m_datastream >> l_b_drive;
        qint8 r_f_drive;
        m_datastream >> r_f_drive;
        qint8 r_m_drive;
        m_datastream >> r_m_drive;
        qint8 r_b_drive;
        m_datastream >> r_b_drive;
        emit driveMotorPowerReceived(l_f_drive, l_m_drive, l_b_drive, r_f_drive, r_m_drive, r_b_drive);
    }
    else if(_packetType == static_cast<quint8>(Types::SWERVE_DRIVE_STATE)) {
        quint8 swerve_state;
        m_datastream >> swerve_state;
        emit swerveDriveStateReceived(swerve_state);
    }
    else if(_packetType == static_cast<quint8>(Types::ARM_MOTORS)) {
        qint8 arm_motor_1;
        m_datastream >> arm_motor_1;
        qint8 arm_motor_2;
        m_datastream >> arm_motor_2;
        qint8 arm_motor_3;
        m_datastream >> arm_motor_3;
        qint8 arm_motor_4;
        m_datastream >> arm_motor_4;
        qint8 arm_motor_5;
        m_datastream >> arm_motor_5;
        emit armMotorsReceived(arm_motor_1, arm_motor_2, arm_motor_3, arm_motor_4, arm_motor_5);
    }
    else if(_packetType == static_cast<quint8>(Types::POTENTIOMETERS)) {
        quint8 pot_1;
        m_datastream >> pot_1;
        quint8 pot_2;
        m_datastream >> pot_2;
        quint8 pot_3;
        m_datastream >> pot_3;
        quint8 pot_4;
        m_datastream >> pot_4;
        quint8 pot_5;
        m_datastream >> pot_5;
        emit potentiometersReceived(pot_1, pot_2, pot_3, pot_4, pot_5);
    }
    else if(_packetType == static_cast<quint8>(Types::SELECT_CAMERA)) {
        quint8 selected_camera;
        m_datastream >> selected_camera;
        emit selectCameraReceived(selected_camera);
    }
    else if(_packetType == static_cast<quint8>(Types::CALLSIGN)) {
    }
    else if(_packetType == static_cast<quint8>(Types::GPS_POSITION)) {
        quint8 gps_valid;
        m_datastream >> gps_valid;
        qint32 latitude;
        m_datastream >> latitude;
        qint32 longitude;
        m_datastream >> longitude;
        quint32 altitude;
        m_datastream >> altitude;
        emit gpsPositionReceived(gps_valid, latitude, longitude, altitude);
    }
    else if(_packetType == static_cast<quint8>(Types::GPS_HEADING)) {
        qint16 gps_heading;
        m_datastream >> gps_heading;
        emit gpsHeadingReceived(gps_heading);
    }
    else if(_packetType == static_cast<quint8>(Types::GPS_SPEED)) {
        quint16 gps_speed;
        m_datastream >> gps_speed;
        emit gpsSpeedReceived(gps_speed);
    }
    else if(_packetType == static_cast<quint8>(Types::MAGNETOMETER)) {
        qint16 mag_x;
        m_datastream >> mag_x;
        qint16 mag_y;
        m_datastream >> mag_y;
        qint16 mag_z;
        m_datastream >> mag_z;
        emit magnetometerReceived(mag_x, mag_y, mag_z);
    }
    else if(_packetType == static_cast<quint8>(Types::ACCELEROMETER)) {
        qint16 accel_x;
        m_datastream >> accel_x;
        qint16 accel_y;
        m_datastream >> accel_y;
        qint16 accel_z;
        m_datastream >> accel_z;
        emit accelerometerReceived(accel_x, accel_y, accel_z);
    }
    else if(_packetType == static_cast<quint8>(Types::GYROSCOPE)) {
        qint16 gyro_x;
        m_datastream >> gyro_x;
        qint16 gyro_y;
        m_datastream >> gyro_y;
        qint16 gyro_z;
        m_datastream >> gyro_z;
        emit gyroscopeReceived(gyro_x, gyro_y, gyro_z);
    }
    else if(_packetType == static_cast<quint8>(Types::DEBUGGING_INFO)) {
    }
    else if(_packetType == static_cast<quint8>(Types::BUILD_INFO)) {
    }

        // error handling
}

// TODO: initial
quint16 PacketHandler::crc(void *data, int size, quint16 initial)
{
    quint16 remainder = 0xFFFF;
    quint8 *bytes = reinterpret_cast<quint8 *>(data);
    for (int i = 0; i < size; i++){
        remainder ^= bytes[i] << 8;
        for (int bit = 8; bit > 0; bit--){
            if (remainder & 0x8000){
                remainder = (remainder << 1) ^ 0x1021;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}

/* @write_slots_source */
void PacketHandler::writePause (quint8 pause_state)
{
    m_datastream << pause_state;
}

void PacketHandler::writeDriveMotorPower (qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive)
{
    m_datastream << l_f_drive << l_m_drive << l_b_drive << r_f_drive << r_m_drive << r_b_drive;
}

void PacketHandler::writeSwerveDriveState (quint8 swerve_state)
{
    m_datastream << swerve_state;
}

void PacketHandler::writeArmMotors (qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5)
{
    m_datastream << arm_motor_1 << arm_motor_2 << arm_motor_3 << arm_motor_4 << arm_motor_5;
}

void PacketHandler::writeServo (quint8 ax12_addr, qint16 ax12_angle)
{
    m_datastream << ax12_addr << ax12_angle;
}

void PacketHandler::writeSelectCamera (quint8 selected_camera)
{
    m_datastream << selected_camera;
}

void PacketHandler::writeCallsign (quint8 callsign_data_length, quint8 callsign_data)
{
    m_datastream << callsign_data_length << callsign_data;
}

void PacketHandler::writeCameraCommand (quint8 camera_data_length, quint8 camera_data)
{
    m_datastream << camera_data_length << camera_data;
}


/* @read_slots_source */
