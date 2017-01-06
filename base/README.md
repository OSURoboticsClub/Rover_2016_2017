# Base Station

This folder contains the source code for the base station.

## System Requirements
The base software is supported on recent versions of Ubuntu. It is written in **Qt 5.7**. For building, the software currently requires CMake, and OpenGL. Very soon building will also require an additional library to handle user inputs from game controllers; most likely this will be the SFML library. 
To install Qt, follow the installation instructions on their [website](https://wiki.qt.io/Install_Qt_5_on_Ubuntu). To install other requirements, run the following: 
```bash
sudo apt-get update && sudo apt-get install 
  build-essential 
  cmake 
  mesa-common-dev 
  libsfml-dev
  ```
