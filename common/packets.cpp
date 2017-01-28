#include "packets.h"
#include <algorithm>
#include <QIODevice>
#include <QDebug>

Packets::Packets(QObject *parent) :
    QObject(parent),
    m_datastream()
{
	m_datastream.setByteOrder(QDataStream::LittleEndian);
}

Packets::~Packets()
{
}

void Packets::setDevice(QIODevice *d)
{
    m_datastream.setDevice(d);
}

QIODevice *Packets::device()
{
    return m_datastream.device();
}

/* start and end bytes, as well as escape bytes removed */
void Packets::parsePacket(quint8 size)
{
    /* datastream head at CRC */
    /* @parse_packets */
    quint16 _read_crc;
    m_datastream >> _read_crc;
    quint8 _packetType;
    m_datastream >> _packetType;
    if(_packetType == (static_cast<quint8>(Types::PAUSE) | 0x80)) {
        quint8 pause_state;
        m_datastream >> pause_state;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&pause_state, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit pauseReceived(pause_state);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::BATTERY_VOLTAGE) | 0x80)) {
        quint16 battery_voltage;
        m_datastream >> battery_voltage;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&battery_voltage, sizeof(quint16), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit batteryVoltageReceived(battery_voltage);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::DRIVE_MOTOR_POWER) | 0x80)) {
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
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&l_f_drive, sizeof(qint8), _crc);
        _crc = crc(&l_m_drive, sizeof(qint8), _crc);
        _crc = crc(&l_b_drive, sizeof(qint8), _crc);
        _crc = crc(&r_f_drive, sizeof(qint8), _crc);
        _crc = crc(&r_m_drive, sizeof(qint8), _crc);
        _crc = crc(&r_b_drive, sizeof(qint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit driveMotorPowerReceived(l_f_drive, l_m_drive, l_b_drive, r_f_drive, r_m_drive, r_b_drive);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::SWERVE_DRIVE_STATE) | 0x80)) {
        quint8 swerve_state;
        m_datastream >> swerve_state;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&swerve_state, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit swerveDriveStateReceived(swerve_state);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::ARM_MOTORS) | 0x80)) {
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
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&arm_motor_1, sizeof(qint8), _crc);
        _crc = crc(&arm_motor_2, sizeof(qint8), _crc);
        _crc = crc(&arm_motor_3, sizeof(qint8), _crc);
        _crc = crc(&arm_motor_4, sizeof(qint8), _crc);
        _crc = crc(&arm_motor_5, sizeof(qint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit armMotorsReceived(arm_motor_1, arm_motor_2, arm_motor_3, arm_motor_4, arm_motor_5);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::POTENTIOMETERS) | 0x80)) {
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
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&pot_1, sizeof(quint8), _crc);
        _crc = crc(&pot_2, sizeof(quint8), _crc);
        _crc = crc(&pot_3, sizeof(quint8), _crc);
        _crc = crc(&pot_4, sizeof(quint8), _crc);
        _crc = crc(&pot_5, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit potentiometersReceived(pot_1, pot_2, pot_3, pot_4, pot_5);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::SELECT_CAMERA) | 0x80)) {
        quint8 selected_camera;
        m_datastream >> selected_camera;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&selected_camera, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit selectCameraReceived(selected_camera);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::CALLSIGN) | 0x80)) {
        char _data[size];
        m_datastream.readRawData(_data, size);
        QByteArray _byte_array = QByteArray(_data);
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&_byte_array, size, _crc);
        if(_crc == _read_crc) {
            emit callsignReceived(_byte_array);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::GPS_POSITION) | 0x80)) {
        quint8 gps_pos_valid;
        m_datastream >> gps_pos_valid;
        qint64 latitude;
        m_datastream >> latitude;
        qint64 longitude;
        m_datastream >> longitude;
        qint32 altitude;
        m_datastream >> altitude;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&gps_pos_valid, sizeof(quint8), _crc);
        _crc = crc(&latitude, sizeof(qint64), _crc);
        _crc = crc(&longitude, sizeof(qint64), _crc);
        _crc = crc(&altitude, sizeof(qint32), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit gpsPositionReceived(gps_pos_valid, latitude, longitude, altitude);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::GPS_TRACK) | 0x80)) {
        quint8 gps_track_valid;
        m_datastream >> gps_track_valid;
        qint16 gps_heading;
        m_datastream >> gps_heading;
        quint16 gps_speed;
        m_datastream >> gps_speed;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&gps_track_valid, sizeof(quint8), _crc);
        _crc = crc(&gps_heading, sizeof(qint16), _crc);
        _crc = crc(&gps_speed, sizeof(quint16), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit gpsTrackReceived(gps_track_valid, gps_heading, gps_speed);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::MAGNETOMETER) | 0x80)) {
        qint16 mag_x;
        m_datastream >> mag_x;
        qint16 mag_y;
        m_datastream >> mag_y;
        qint16 mag_z;
        m_datastream >> mag_z;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&mag_x, sizeof(qint16), _crc);
        _crc = crc(&mag_y, sizeof(qint16), _crc);
        _crc = crc(&mag_z, sizeof(qint16), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit magnetometerReceived(mag_x, mag_y, mag_z);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::ACCELEROMETER) | 0x80)) {
        qint16 accel_x;
        m_datastream >> accel_x;
        qint16 accel_y;
        m_datastream >> accel_y;
        qint16 accel_z;
        m_datastream >> accel_z;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&accel_x, sizeof(qint16), _crc);
        _crc = crc(&accel_y, sizeof(qint16), _crc);
        _crc = crc(&accel_z, sizeof(qint16), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit accelerometerReceived(accel_x, accel_y, accel_z);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::GYROSCOPE) | 0x80)) {
        qint16 gyro_x;
        m_datastream >> gyro_x;
        qint16 gyro_y;
        m_datastream >> gyro_y;
        qint16 gyro_z;
        m_datastream >> gyro_z;
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&gyro_x, sizeof(qint16), _crc);
        _crc = crc(&gyro_y, sizeof(qint16), _crc);
        _crc = crc(&gyro_z, sizeof(qint16), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
            emit gyroscopeReceived(gyro_x, gyro_y, gyro_z);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::DEBUGGING_INFO) | 0x80)) {
        char _data[size];
        m_datastream.readRawData(_data, size);
        QByteArray _byte_array = QByteArray(_data);
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&_byte_array, size, _crc);
        if(_crc == _read_crc) {
            emit debuggingInfoReceived(_byte_array);
        }
    }
    else if(_packetType == (static_cast<quint8>(Types::BUILD_INFO) | 0x80)) {
        char _data[size];
        m_datastream.readRawData(_data, size);
        QByteArray _byte_array = QByteArray(_data);
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        _crc = crc(&_byte_array, size, _crc);
        if(_crc == _read_crc) {
            emit buildInfoReceived(_byte_array);
        }
    }
    else if(_packetType == static_cast<quint8>(Types::PAUSE)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::DRIVE_MOTOR_POWER)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::SWERVE_DRIVE_STATE)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::ARM_MOTORS)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::SERVO)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::SELECT_CAMERA)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::CALLSIGN)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }
    else if(_packetType == static_cast<quint8>(Types::CAMERA_COMMAND)) {
        quint16 _crc = 0xFFFF;
        _crc = crc(&_packetType, sizeof(quint8), _crc);
        if(_crc == _read_crc) {
            qDebug() << "parsed a packet";
        }
    }

        // error handling
}

quint16 Packets::crc(void *data, int size, quint16 initial)
{
    quint16 remainder = initial;
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
void Packets::writePause(quint8 pause_state)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::PAUSE);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&pause_state, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)4;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << pause_state;
}

void Packets::writeDriveMotorPower(qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::DRIVE_MOTOR_POWER);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&l_f_drive, sizeof(qint8), _crc);
    _crc = crc(&l_m_drive, sizeof(qint8), _crc);
    _crc = crc(&l_b_drive, sizeof(qint8), _crc);
    _crc = crc(&r_f_drive, sizeof(qint8), _crc);
    _crc = crc(&r_m_drive, sizeof(qint8), _crc);
    _crc = crc(&r_b_drive, sizeof(qint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)9;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << l_f_drive;
    m_datastream << l_m_drive;
    m_datastream << l_b_drive;
    m_datastream << r_f_drive;
    m_datastream << r_m_drive;
    m_datastream << r_b_drive;
}

void Packets::writeSwerveDriveState(quint8 swerve_state)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::SWERVE_DRIVE_STATE);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&swerve_state, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)4;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << swerve_state;
}

void Packets::writeArmMotors(qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::ARM_MOTORS);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&arm_motor_1, sizeof(qint8), _crc);
    _crc = crc(&arm_motor_2, sizeof(qint8), _crc);
    _crc = crc(&arm_motor_3, sizeof(qint8), _crc);
    _crc = crc(&arm_motor_4, sizeof(qint8), _crc);
    _crc = crc(&arm_motor_5, sizeof(qint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)8;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << arm_motor_1;
    m_datastream << arm_motor_2;
    m_datastream << arm_motor_3;
    m_datastream << arm_motor_4;
    m_datastream << arm_motor_5;
}

void Packets::writeServo(quint8 ax12_addr, qint16 ax12_angle)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::SERVO);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&ax12_addr, sizeof(quint8), _crc);
    _crc = crc(&ax12_angle, sizeof(qint16), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)6;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << ax12_addr;
    m_datastream << ax12_angle;
}

void Packets::writeSelectCamera(quint8 selected_camera)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::SELECT_CAMERA);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&selected_camera, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)4;
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream << selected_camera;
}

void Packets::writeCallsign(QByteArray callsign_data)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::CALLSIGN);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&callsign_data, sizeof(QByteArray), _crc);
    m_datastream << (quint8)1;
    m_datastream << static_cast<quint8>(callsign_data.size() + 3);
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream.writeRawData(callsign_data.constData(), callsign_data.size());
}

void Packets::writeCameraCommand(QByteArray camera_data)
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::CAMERA_COMMAND);
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    _crc = crc(&camera_data, sizeof(QByteArray), _crc);
    m_datastream << (quint8)1;
    m_datastream << static_cast<quint8>(camera_data.size() + 3);
    m_datastream << _crc;
    m_datastream << _packetType;
    m_datastream.writeRawData(camera_data.constData(), camera_data.size());
}



/* @read_slots_source */
void Packets::readPause()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::PAUSE) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readBatteryVoltage()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::BATTERY_VOLTAGE) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readDriveMotorPower()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::DRIVE_MOTOR_POWER) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readSwerveDriveState()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::SWERVE_DRIVE_STATE) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readArmMotors()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::ARM_MOTORS) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readPotentiometers()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::POTENTIOMETERS) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readSelectCamera()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::SELECT_CAMERA) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readCallsign()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::CALLSIGN) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readGpsPosition()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::GPS_POSITION) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readGpsTrack()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::GPS_TRACK) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readMagnetometer()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::MAGNETOMETER) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readAccelerometer()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::ACCELEROMETER) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readGyroscope()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::GYROSCOPE) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readDebuggingInfo()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::DEBUGGING_INFO) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

void Packets::readBuildInfo()
{
    qDebug() << "writing a packet";
    quint8 _packetType = static_cast<quint8>(Types::BUILD_INFO) | 0x80;
    quint16 _crc = 0xFFFF;
    _crc = crc(&_packetType, sizeof(quint8), _crc);
    m_datastream << (quint8)1;
    m_datastream << (quint8)3;
    m_datastream << _crc;
    m_datastream << _packetType;
}

