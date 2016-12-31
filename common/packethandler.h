#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>
#include <QtGlobal>



class PacketHandler : public QObject
{
    Q_OBJECT
    /*
        @datastream: m_datastream
        @types_enum: Types
        @crc: crc
    */
public:
    PacketHandler(QObject *parent = 0);
    explicit PacketHandler(QIODevice *d, QObject *parent = 0);
    PacketHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    ~PacketHandler();

    void parsePacket(quint8 size);

    QIODevice *device() const;
    void setDevice(QIODevice *d);
    void unsetDevice();

    enum class Types: quint8 {
        /* @packet_types_header */
        COMMAND_NOT_RECOGNIZED = 0,
        PAUSE = 5,
        BATTERY_VOLTAGE = 6,
        DRIVE_MOTOR_POWER = 16,
        SWERVE_DRIVE_STATE = 17,
        ARM_MOTORS = 18,
        POTENTIOMETERS = 19,
        SERVO = 20,
        SELECT_CAMERA = 32,
        CALLSIGN = 33,
        CAMERA_COMMAND = 34,
        GPS_POSITION = 35,
        GPS_HEADING = 36,
        GPS_SPEED = 37,
        MAGNETOMETER = 38,
        ACCELEROMETER = 39,
        GYROSCOPE = 40,
        DEBUGGING_INFO = 112,
        BUILD_INFO = 113,
    };

signals:
    /* @recieved_signals_header */
    void pauseReceived(quint8 pause_state);
    void batteryVoltageReceived(quint16 battery_voltage);
    void driveMotorPowerReceived(qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive);
    void swerveDriveStateReceived(quint8 swerve_state);
    void armMotorsReceived(qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5);
    void potentiometersReceived(quint8 pot_1, quint8 pot_2, quint8 pot_3, quint8 pot_4, quint8 pot_5);
    void selectCameraReceived(quint8 selected_camera);
    void callsignReceived(quint8 callsign_data_length, quint8 callsign_data);
    void gpsPositionReceived(quint8 gps_valid, qint32 latitude, qint32 longitude, quint32 altitude);
    void gpsHeadingReceived(qint16 gps_heading);
    void gpsSpeedReceived(quint16 gps_speed);
    void magnetometerReceived(qint16 mag_x, qint16 mag_y, qint16 mag_z);
    void accelerometerReceived(qint16 accel_x, qint16 accel_y, qint16 accel_z);
    void gyroscopeReceived(qint16 gyro_x, qint16 gyro_y, qint16 gyro_z);
    void debuggingInfoReceived(quint8 debug_str_length, quint8 debug_str_data);
    void buildInfoReceived(quint8 build_info_length, quint8 build_info_data);

public slots:
    /* @write_slots_header */
    void writePause(quint8 pause_state);
    void writeDriveMotorPower(qint8 l_f_drive, qint8 l_m_drive, qint8 l_b_drive, qint8 r_f_drive, qint8 r_m_drive, qint8 r_b_drive);
    void writeSwerveDriveState(quint8 swerve_state);
    void writeArmMotors(qint8 arm_motor_1, qint8 arm_motor_2, qint8 arm_motor_3, qint8 arm_motor_4, qint8 arm_motor_5);
    void writeServo(quint8 ax12_addr, qint16 ax12_angle);
    void writeSelectCamera(quint8 selected_camera);
    void writeCallsign(quint8 callsign_data_length, quint8 callsign_data);
    void writeCameraCommand(quint8 camera_data_length, quint8 camera_data);

    /* @read_slots_header */
    void readPause();
    void readBatteryVoltage();
    void readDriveMotorPower();
    void readSwerveDriveState();
    void readArmMotors();
    void readPotentiometers();
    void readSelectCamera();
    void readCallsign();
    void readGpsPosition();
    void readGpsHeading();
    void readGpsSpeed();
    void readMagnetometer();
    void readAccelerometer();
    void readGyroscope();
    void readDebuggingInfo();
    void readBuildInfo();

private:
   Types m_types;
   QDataStream m_datastream;
   quint16 crc(void *data, int size, quint16 initial = 0);
};

#endif // PACKETHANDLER_H
