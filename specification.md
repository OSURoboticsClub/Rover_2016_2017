The Xtend radios provide a bidirectional pipe between the rover and the computer (similar to a serial cable). Data is sent LSB first, just as in RS-232. This byte stream is divided into commands by special bytes. Each command has the following format:
  <start byte>, <command byte>, [0 or more data bytes]
Commands are delineated with start bytes. If the rover encounters a start byte in the middle of a command, it discards the current command and begins receiving the new one. When a verbatim start byte needs to be transmitted as part of the data, it is prefixed with an escape byte. If an escape byte needs to be transmitted verbatim, it is also prefixed with an escape byte. The start and escape bytes are:
  Start - 0x01
  Escape - 0x02
Commands are sent from the computer to the rover, and result in a reply from the rover to the computer. All commands read or write values in the rover firmware. The MSB (bit 7, 0x80) of the command indicates whether that command is reading or writing. When reading, the command should contain no data. Commands are acted upon as soon as the required number of data bytes for the command have been received. 
For each command, the rover sends a reply containing the command (in the same read/write form as it received it) and any additional data. For write commands, no data is sent in the reply. For read commands, the requested data is sent in the reply. If the rover receives a command it does not recognize, it sends a reply with a command byte of 0x00.
 
Multi-byte values are transmitted little-endian. Two’s complement is used for signed values.
In the following table, argument specifications begin with a code (i8 = 8-bit int, u16 = 16-bit unsigned int, etc) indicating the size of each argument. The leftmost argument is the first to the sent down the pipe.
Camera video data is transmitted through a separate interface. Camera commands, and camera selection is set through this command protocol.

| Name | RW | Command Code | Arguments | Default values | Notes | 
| ---- | --- | ------------ | --------- | -------------- | ----- | 
| Command not Recognized | - | 0x00 | None | None | Sent as a reply to unknown commands | 
| Pause | RW | 0x05 | state | 1 | 0 = pause (no rover motion) 1 = unpause | 
| Battery voltage | R | 0x06 | u16 voltage | None | Battery voltage in mV | 
| Drive Motor Power | RW | 0x10 | i8 left_motor, i8 right_motor | 0,0 | -127 = full reverse 128 = full | 
| Swerve Drive State | RW | 0x11 | u8 state | 0 | 0x00 = Off (no motion), 0x01 = Straight, 0x02 = Turn | 
| Select Camera | RW | 0x20 | u8 camera | 0 | 0-3; select camera feed to send to the base station and to send commands to. TODO: define which camera corresponds to which number | 
| Camera Command | W | 0x30 | Command for camera | u8 length, * data | Custom camera commands defined in camera manual. Length defines the number of data bytes in the command (0-255). Data is the command to be sent to the camera. The command is sent once the specified number of bytes have | 
