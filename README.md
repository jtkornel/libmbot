# libmbot (work in progress)
C++ library for interfacing with mbot robots.

Libmbot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication over serial port. For now it supports communicating with the standard Makeblock firmware.

As a higher-level abstraction devices can be grouped into board objects which helps configuring built-in ports and slots and validate externally connected devices.


## How to install

Libmbot depends on ament_cmake from ROS2, plus an ament fork of the wjwwood serial library, and is easiest to build as a part of a ROS2 workspace.

For instance add to your workspace .repo file
```
  serial:
    type: git
    url: https://github.com/RoverRobotics-forks/serial-ros2
    version: master
  libmbot:
    type: git
    url: https://github.com/jtkornel/libmbot
    version: main

```

You can then download and build the library and its dependency
```
vcs import src < my_workspace.repos

colcon build --symlink-install
```