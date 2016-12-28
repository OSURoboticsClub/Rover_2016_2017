# Radio Communication Specification
## Packet Structure
The Xtend radios provide a bidirectional pipe between the rover and the computer (similar to a serial cable). Data is sent LSB first, just as in RS-232. This byte stream is divided into commands by special bytes. Each command has the following format:

> \<start byte\>, \<length byte\>, \<2 byte CRC\>, \<command byte\>, [0 or more data bytes]

Commands are delineated with start bytes. Following the start byte is a length byte that encodes the size of the packet, which is the size of all packet data after this byte (including the 2 byte CRC, command byte, and data bytes). Encoding the length allows for unambiguously delineating the boundaries of the packet. This strategy avoids the use of escape bytes, and ensures that packets with fixed size arguments are always the same size. Following the length byte is a two byte circular redundancy check (CRC) value that allows for checking if any packet data has been corrupted during transmission. The command byte and the data bytes are used as inputs into the algorithm. Details on the implementation of the CRC are in the code. Finally, a command byte determines the type of the encoded packet (see Packet Type Specification), followed by packet data.

Special bytes:
  Start - 0x01

## Packet Types
Commands are sent from the computer to the rover, and result in a reply from the rover to the computer. Command execution and the reply occur after the entire length of the packet is received and verified against the CRC. Conceptually, commands can be thought of as reading or writing values in the rover's memory. The MSB (bit 7, 0x80) of the command indicates whether that command is reading or writing (0 = writing, 1 = reading). When reading, the command should contain no data.
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
