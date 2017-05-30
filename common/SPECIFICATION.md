# Radio Communication Specification
## Packet Structure
The data radios provide a bidirectional pipe between the rover and the computer (similar to a serial cable). Data is sent LSB first, just as in RS-232. This byte stream is divided into command packets. Each command packet has the following format:

```
<start byte (0x01)>, <length byte>, <2 byte CRC (little-endian)>, <command byte>, [0 or more data bytes]
└────────────────────────Packet Header──────────────────────────┘└────────────Packet Body──────────────┘
```

A start byte (value 0x01) begins each packet. (Before receiving a start byte, the receiver should ignore all non-start input bytes.) Following the start byte is a single byte indicating the packet length (as an unsigned integer). The length takes into account all following bytes (the CRC, command bytes, and data byte(s)). After the length byte is the CRC, which is used to validate the contents of the packet after reception. (See below for details on CRC calculation. Packets with an invalid CRC should be silently ignored.) Collectively, the start byte, length byte, and CRC are the packet header.

Following the packet header is the packet body, which is composed of a command byte (see Packet Types) and zero or more data bytes. Up to 127 bytes of packet data are permitted. The number and meaning of data bytes depends on the command byte, but will be reflected in the length byte in the header.

Special bytes:
  Start - 0x01

## CRC Calculation
The CRC is 16-bit with a polynomial of 0x1021 and an initial value 0xFFFF. The CRC is calculated over the packet body (command byte and data byte(s)). The following function will calculate the CRC:

```c
uint16_t calc_crc(uint8_t *body, int body_length){
	uint16_t remainder = 0xFFFF;
	for (int i = 0; i < body_length; i++){
		remainder ^= body[i] << 8;
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
```

After the CRC has been calculated, it should be checked for agreement with the CRC in the packet header. If they disagree, the packet should be silently discarded.

## Packet Types
Commands are sent from the computer to the rover, and result in a reply from the rover to the computer. Command execution and the reply occur after the entire length of the packet is received and verified against the CRC. Conceptually, commands can be thought of as reading or writing values in the rover's memory. The MSB (bit 7, 0x80) of the command byte indicates whether that command is reading or writing (0 = writing, 1 = reading). When reading, the command should contain no data.
For each command, the rover sends a reply containing the command (in the same read/write form as it received it) and any additional data. For write commands, no data is sent in the reply. For read commands, the requested data is sent in the reply. If the rover receives a command it does not recognize, it sends a reply with a command byte of 0x00. If the computer attempts to write a read-only register, the rover acts as if the write succeeded.
 
Multi-byte values are transmitted little-endian. Two’s complement is used for signed values.
In the following table, argument specifications begin with a code (i8 = 8-bit int, u16 = 16-bit unsigned int, etc) indicating the size of each argument. Any variable length arguments are specified using a star (\*). The leftmost argument is the first in the packet data.
Camera video data is transmitted through a separate interface. Camera commands and camera selection are sent through this command protocol.

Note: when adding commands to the table, each argument's name must be unique among all commands. Since they are used in the rover firmware,
don't change the name of existing command arguments.

## Packet Type Specification
| Name | RW | Command Code | Arguments | Default values | Notes | 
| ---- | --- | ------------ | --------- | -------------- | ----- | 
| Command not Recognized   | -  | 0x00 | u8 wrong_command | - | Sent as a reply to unknown commands. |
| Pause                    | RW | 0x05 | u8 pause_state | 1 | 0 = pause (no rover motion) 1 = unpause |
| Battery Voltage          | R  | 0x06 | u16 battery_voltage | - | Battery voltage in mV |
| Drive Motor Power        | RW | 0x10 | i8 l_f_drive, i8 l_m_drive, i8 l_b_drive, i8 r_f_drive, i8 r_m_drive, i8 r_b_drive | 0,0,0,0,0,0 | -127 = full reverse 128 = full forward, r = right, l = left, f = front, m = middle, b = back |
| Swerve Drive State       | RW | 0x11 | u8 swerve_state | 0 | 0x00 = Off (no motion), 0x01 = Straight, 0x02 = Turn |
| Arm Motors               | RW | 0x12 | i8 arm_motor_1, i8 arm_motor_2, i8 arm_motor_3, i8 arm_motor_4, i8 arm_motor_5 | 0,0,0,0,0 | -127 = full reverse 128 = full forward TODO: Define motor->joint mapping
| Servo                    |  W | 0x14 | u8 ax12_addr, u16 ax12_angle | 0,512 | Set the target angle of an AX12 servo. |
| S Bus Values 1           | R  | 0x15 | u16 sbus_1, u16 sbus_2, u16 sbus_3, u16 sbus_4, u16 sbus_5, u16 sbus_6, u16 sbus_7, u16 sbus_8 | - | S-Bus channel values. |
| S Bus Values 2           | R  | 0x16 | u16 sbus_9, u16 sbus_10, u16 sbus_11, u16 sbus_12, u16 sbus_13, u16 sbus_14, u16 sbus_15, u16 sbus_16, u8 sbus_active | - | S-Bus channel values plus a status bit indicating if the failsafe has been set. |
| Select Camera            | RW | 0x20 | u8 selected_camera | 5 | 1-6; select camera feed to send to the base station and to send commands to. Note: camera output will be disabled unless the callsign has been set. TODO: define which camera corresponds to which number.  |
| Callsign                 | RW | 0x21 | u8 callsign_data_length, * callsign_data| - | ASCII string of callsign (use numerals and capital letters only) |
| Camera Command           |  W | 0x22 | u8 camera_data_length, * camera_data | - | Custom camera commands defined in camera manual. camera_data_length defines the number of data bytes in the command (0-255). camera_data is the command to be sent to the camera.|
| GPS Position             | R  | 0x23 | u8 gps_pos_valid, i64 latitude, i64 longitude, i32 altitude | 0,0,0,0 | GPS Position. Good when valid != 0. Sign meaning: +=north/east. Latitude and longitude are in 10e-6 minute units. Altitude is in meters. |
| GPS Track                | R  | 0x24 | u8 gps_track_valid, i16 gps_heading, u16 gps_speed | 0,0,0 | GPS Heading, in hundredths of a degree. Speed in meters/hour. |
| Magnetometer             | R  | 0x26 | i16 mag_x, i16 mag_y, i16 mag_z | 0,0,0 | External magnetometer reading. TODO: define units and axis directions. |
| Accelerometer            | R  | 0x27 | i16 accel_x, i16 accel_y, i16 accel_z | 0,0,0 | IMU accelerometer reading. TODO: define units and axis directions. |
| Gyroscope                | R  | 0x28 | i16 gyro_x, i16 gyro_y, i16 gyro_z | 0,0,0 | IMU gyroscope reading. TODO: define units and axis directions. |
| Compass Heading          | R  | 0x29 | u8 compass_heading_valid, i16 compass_heading | 0,0 | Magnetic compass heading. TODO: define units and axis directions. |
| Pan Tilt Speed           | RW | 0x2B | i8 pan_speed, i8 tilt_speed | 0,0 | Pan/tilt speeds. Both range from -128 to 127. The total angle will be limited to safe values.|
| AX12 Arm Mode            | RW | 0x2C | u8 arm_mode | 0 | 0 = arm AX12s not in use, 1 = arm ax12s in use |
| End Effector Speed       | RW | 0x2D | i16 ee_speed | 0 | Speed of end effector pitch control. Range -1023 to 1023. |
| Grabber                  | RW | 0x2E | i16 grabber_speed, i16 grabber_rotation_speed | 0,0 | Speed of grabber motors. Range -1023 to 1023. |
| Container Sealer         | RW | 0x2F | u16 cflex1_angle, u16 cflex2_angle, i16 clid_speed | 500,500,0 | Angles and speed of container motors. |
| GPIO Read State          | R  | 0x32 | u8 gpio_state | 0 | GPIO pin directions. 1 = high, 0 = low. Mapping: MSB X X 5 4 3 2 1 X LSB. |
| Sample Camera Action     | RW | 0x35 | u8 cam_action | 0 | Sample cam actions. 0 = none, 4 = shutter, 3 = focus, 1 = zoom in, 2 = zoom out |
| Navigation Camera Action | RW | 0x36 | u8 nav_action | 0 | Naviagation cam actions. 0 = none, 1 = zoom in, 2 = zoom out |
| Soil Sensor Send         |  W | 0x40 | u8 soil_send_data_length, * soil_send_data| - | Data string to send to the soil sensor. |
| Soil Sensor Recv         | RW | 0x41 | u8 soil_recv_data_length, * soil_recv_data| - | Reply string received from the soil sensor. |
| Joystick                 | RW | 0x50 | i8 fr_joylh, i8 fr_joylv, i8 fr_joyrh, i8 fr_joyrv, i8 fr_potl, i8 fr_potr, i8 fr_sidel, i8 fr_sider, u8 fr_buttons, i8 xbox_joylh, i8 xbox_joylv, i8 xbox_joyrh, i8 xbox_joyrv, i8 xbox_triggerl, i8 xbox_triggerr, u8 xbox_buttons_high, u8 xbox_buttons_low | 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 | Frsky buttons are packed (MSB) H ..A (LSB). MSB of xbox_buttons_high is enable; must be 1! XBOX low byte -> (MSB)(RSC)(LSC)(RB)(LB)(Y)(X)(B)(A)(LSB) XBOX high byte (MSB)(1)(DPD)(DPU)(DPR)(DPL)(HOME)(STRT)(SEL)(LSB)|
| Autonomous Enable        | RW | 0x60 | u8 auton_en | 0 | If 1, do autonomous traversal. |
| Autonomous Waypoint 1    | RW | 0x61 | i64 auton_way1_lat, i64 auton_way1_lon, u16 auton_way1_speed | 0,0,0 | Autonomous target waypoint. Speed in mm/s. |
| Autonomous Waypoint 2    | RW | 0x63 | i64 auton_way2_lat, i64 auton_way2_lon, u16 auton_way2_speed | 0,0,0 | Autonomous next waypoint. Speed in mm/s. |
| Time ms                  | R  | 0x64 | u32 time_ms | 0 | Time since rover was turned on, in ms. |
