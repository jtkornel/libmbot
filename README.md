# libmbot (work in progress)
C++ library for interfacing with mbot robots.

Libmbot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication over serial port. For now it supports communicating with the standard Makeblock firmware.

As a higher-level abstraction devices can be grouped into board objects which helps configuring built-in ports and slots and validate externally connected devices.

The library has primarily been tested on Linux. Mac support may be possible, but I have experienced unreliable communication for the USB-Serial bridge on that platform.

## How to install

Libmbot uses CMake as its build system, and can be built standalone or as part of a ROS2 workspace. It depends on ASIO for serial communication, which is automatically downloaded using FetchContent in the CMake file.

To build in ROS2 you can for instance add to your workspace .repo file
```
  libmbot:
    type: git
    url: https://github.com/jtkornel/libmbot
    version: main

```
And then to download the sourcecode and build:
```
vcs import src < my_workspace.repos

colcon build --symlink-install
```
