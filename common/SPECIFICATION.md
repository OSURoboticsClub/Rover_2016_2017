# Radio Communication Specification
## Packet Structure
The Xtend radios provide a bidirectional pipe between the rover and the computer (similar to a serial cable). Data is sent LSB first, just as in RS-232. This byte stream is divided into commands by special bytes. Each command has the following format:

> \<start byte (0x01)\>, \<length byte\>, \<2 byte CRC (little-endian)\>, \<command byte\>, [0 or more data bytes]

A start byte (value 0x01) begins each command. (Before receiving a start byte, the receiver should ignore all non-start input bytes.) Following the start byte is a single byte indicating the packet length (as an unsigned integer). The length takes into account all following bytes (the CRC, command bytes, and data byte(s)). After the length byte is the CRC, which is used to validate the contents of the packet after reception. (See below for details on CRC calculation. Packets with an invalid CRC should be silently ignored.) Collectively, the start byte, length byte, and CRC are the packet header.

Following the packet header is the packet body, which is composed of a command byte (see Packet Types) and zero or more data bytes. The number and meaning of data bytes depends on the command byte, but will be reflected in the length byte in the header.

Special bytes:
  Start - 0x01

## CRC Calculation
The CRC is 16-bit with a polynomial of 0x1021 and an initial value 0xFFFF. The CRC is calculated over the packet body (command byte and data byte(s)). The following function will calculate the CRC:

```
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
In the following table, argument specifications begin with a code (i8 = 8-bit int, u16 = 16-bit unsigned int, etc) indicating the size of each argument. Any variable length arguments are specified using a star (\*). The leftmost argument is the first to the sent down the pipe.
Camera video data is transmitted through a separate interface. Camera commands and camera selection are sent through this command protocol.

Note: when adding commands to the table, each argument's name must be unique among all commands. Since they are used in the rover firmware,
don't change the name of existing command arguments.

## Packet Type Specification
| Name | RW | Command Code | Arguments | Default values | Notes | 
| ---- | --- | ------------ | --------- | -------------- | ----- | 
| Command not Recognized | -  | 0x00 | u8 wrong_command | - | Sent as a reply to unknown commands. |
| Pause                  | RW | 0x05 | u8 pause_state | 1 | 0 = pause (no rover motion) 1 = unpause | 
| Battery Voltage        | R  | 0x06 | u16 battery_voltage | - | Battery voltage in mV | 
| Drive Motor Power      | RW | 0x10 | i8 l_f_drive, i8 l_m_drive, i8 l_b_drive, i8 r_f_drive, i8 r_m_drive, i8 r_b_drive | 0,0,0,0,0,0 | -127 = full reverse 128 = full forward, r = right, l = left, f = front, m = middle, b = back | 
| Swerve Drive State     | RW | 0x11 | u8 swerve_state | 0 | 0x00 = Off (no motion), 0x01 = Straight, 0x02 = Turn | 
| Arm Motors             | RW | 0x12 | i8 arm_motor_1, i8 arm_motor_2, i8 arm_motor_3, i8 arm_motor_4, i8 arm_motor_5 | 0,0,0,0,0 | -127 = full reverse 128 = full forward TODO: Define motor->joint mapping
| Potentiometers         | R  | 0x13 | u8 pot_1, u8 pot_2, u8 pot_3, u8 pot_4, u8 pot_5 | 0,0,0,0,0 | Potentiometer readings |
| Servo                  |  W | 0x14 | u8 ax12_addr, i16 ax12_angle | 0,0 | Set the target angle of an AX12 servo. |
| Select Camera          | RW | 0x20 | u8 selected_camera | 0 | 0-5; select camera feed to send to the base station and to send commands to. Note: camera output will be disabled unless the callsign has been set. TODO: define which camera corresponds to which number.  |
| Callsign               | RW | 0x21 | * callsign_data| - | ASCII string of callsign (use numerals and capital letters only) |
| Camera Command         |  W | 0x22 | * camera_data | - | Custom camera commands defined in camera manual. camera_data_length defines the number of data bytes in the command (0-255). camera_data is the command to be sent to the camera.|
| GPS Position           | R  | 0x23 | u8 gps_valid, i64 latitude, i64 longitude, u32 altitude | 0,0,0,0 | GPS Position. Good when valid != 0. Sign meaning: +=north/east. Latitude and longitude are in 10e-6 minute units. Altitude is in meters. |
| GPS Heading            | R  | 0x24 | i16 gps_heading | 0 | GPS Heading, in hundredths of a degree. Check GPS Position for validity. |
| GPS Speed              | R  | 0x25 | u16 gps_speed   | 0 | GPS Speed, in m/h. Check GPS Position for validity. |
| Magnetometer           | R  | 0x26 | i16 mag_x, i16 mag_y, i16 mag_z | 0,0,0 | External magnetometer reading. TODO: define units and axis directions. |
| Accelerometer          | R  | 0x27 | i16 accel_x, i16 accel_y, i16 accel_z | 0,0,0 | IMU accelerometer reading. TODO: define units and axis directions. |
| Gyroscope              | R  | 0x28 | i16 gyro_x, i16 gyro_y, i16 gyro_z | 0,0,0 | IMU gyroscope reading. TODO: define units and axis directions. |
| Debugging Info         | R  | 0x70 | * debug_str_data  | - | Read out the latest debug message. |
| Build Info             | R  | 0x71 | * build_info_data | - | Read out string describing when/how the firmware was built. |
