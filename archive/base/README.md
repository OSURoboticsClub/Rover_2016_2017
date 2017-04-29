# Base Station

This folder contains the source code for the base station.

## System Requirements
The base software is supported on recent versions of Ubuntu. It is written in **Qt 5.7**. For building, the software currently requires CMake, and OpenGL. Very soon building will also require an additional library to handle user inputs from game controllers; most likely this will be the SFML library. 
To install Qt, follow the installation instructions on their [website](https://wiki.qt.io/Install_Qt_5_on_Ubuntu). To install other requirements, run the following: 
```bash
sudo apt-get update && sudo apt-get install \
  build-essential \
  cmake \
  mesa-common-dev \
  libsfml-dev
  ```
## Design Overview
### Serial
The specification file found in the ```common``` folder defines the serial communication protocol used for sending data between the rover and the base station. To implement this protocol, the ```basepackets.py``` file in the ```common``` folder can parse the specification file to extract individual packet types. During the build process, this file completes the ```Packets``` class based on the information provided in the specification. Specifically, the skeleton files contain insertion points, designated by an ```@```, where the parser will insert the necessary code. The ```Packets``` class contains slots for sending packets and signals that are emitted when a packet is received. Once this class is connected to a device, calling the ```parsePackets``` function can deserialize the recieved packet. 
The ```SerialHandler``` class contains an event loop that waits for received packets, and calls on the ```Packets``` class to parse the packet when it is ready. It is a subclass of ```QThread``` and a singleton. To access the class, call the ```instance``` static method, and then the ```p``` method to access the underlying ```Packets``` class. The ```SerialHandler``` class is the interface to use for accessing methods related to serial communication.
### User Inputs
The ```AbstractController``` class defines a base class for all controllers. The purpose of these classes is to provide intuitive access to input values for individual controllers, as well as a set of signals for changes in the controller state. The class uses the SFML library as the joystick backend. Much of this class is devoted to mapping the indicies provided by the SFML library to names that can be easily understood. Additionally, the class contains an ```emitChanges``` method that will cause signals to emit if a change in the joystick is detected. Since this class does not contain an event loop, this method needs to be called explicilty.
The ```ControllerHandler``` class handles the collection of available controllers connected to a computer. It stores a list of controller instances that can be connected to specific controller roles. The class contains an event loop that listens for changes in the controllers, and calls corresponding slots when a change is detected.
