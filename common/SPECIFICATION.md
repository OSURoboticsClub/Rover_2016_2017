# Radio Communication Specification
The Xtend radios provide a bidirectional pipe between the rover and the computer (similar to a serial cable). Data is sent LSB first, just as in RS-232. This byte stream is divided into commands by special bytes. Each command has the following format:

\<start byte\>, \<command byte\>, [0 or more data bytes], \<end byte\>
  
Commands are delineated with start bytes and end bytes. If the rover encounters a start byte in the middle of a command, it discards the current command and begins receiving the new one. When a verbatim start or end byte needs to be transmitted as part of the data, it is prefixed with an escape byte. If an escape byte needs to be transmitted verbatim, it is also prefixed with an escape byte. The special bytes are:
  
Start - 0x01  
Escape - 0x02
End - 0x03

Commands are sent from the computer to the rover, and result in a reply from the rover to the computer. Command execution and the reply occur after an end byte is received. Conceptually, commands can be thought of as reading or writing values in the rover's memory. The MSB (bit 7, 0x80) of the command indicates whether that command is reading or writing (0 = writing, 1 = reading). When reading, the command should contain no data. Commands are acted upon as soon as the end byte is received.
For each command, the rover sends a reply containing the command (in the same read/write form as it received it) and any additional data. For write commands, no data is sent in the reply. For read commands, the requested data is sent in the reply. If the rover receives a command it does not recognize, it sends a reply with a command byte of 0x00. If the computer attempts to write a read-only register, the rover acts as if the write succeeded.
 
Multi-byte values are transmitted little-endian. Two’s complement is used for signed values.
In the following table, argument specifications begin with a code (i8 = 8-bit int, u16 = 16-bit unsigned int, etc) indicating the size of each argument. The leftmost argument is the first to the sent down the pipe.
Camera video data is transmitted through a separate interface. Camera commands and camera selection are sent through this command protocol.

Note: when adding commands to the table, each argument's name must be unique among all commands. Since they are used in the rover firmware,
don't change the name of existing command arguments.

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
| Callsign               | RW | 0x21 | u8 callsign_data_length, * callsign_data| - | ASCII string of callsign (use numerals and capital letters only) |
| Camera Command         |  W | 0x22 | u8 camera_data_length, * camera_data | - | Custom camera commands defined in camera manual. camera_data_length defines the number of data bytes in the command (0-255). camera_data is the command to be sent to the camera.|
| GPS Position           | R  | 0x23 | u8 gps_valid, i32 latitude, i32 longitude, u32 altitude| 0,0,0,0 | GPS Position. Good when valid != 0. TODO: define units. |
| GPS Heading            | R  | 0x24 | i16 gps_heading | 0 | GPS Heading. Check GPS Position for validity. TODO: define units. |
| GPS Speed              | R  | 0x25 | u16 gps_speed   | 0 | GPS Speed, in cm/s. Check GPS Position for validity. |
| Magnetometer           | R  | 0x26 | i16 mag_x, i16 mag_y, i16 mag_z | 0,0,0 | External magnetometer reading. TODO: define units and axis directions. |
| Accelerometer          | R  | 0x27 | i16 accel_x, i16 accel_y, i16 accel_z | 0,0,0 | IMU accelerometer reading. TODO: define units and axis directions. |
| Gyroscope              | R  | 0x28 | i16 gyro_x, i16 gyro_y, i16 gyro_z | 0,0,0 | IMU gyroscope reading. TODO: define units and axis directions. |
| Debugging Info         | R  | 0x70 | u8 debug_str_length, * debug_str_data  | - | Read out the latest debug message. |
| Build Info             | R  | 0x71 | u8 build_info_length, * build_info_data | - | Read out string describing when/how the firmware was built. |