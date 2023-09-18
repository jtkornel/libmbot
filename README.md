# libmbot
C++ library for interfacing with mBot robots over a serial port.

Libmbot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication with each device. In other words it exposes a remote method interface which allows writing functionality on host without updating the mBot firmware. For now it supports communicating with the standard Makeblock firmware. This should give support for most sensors available, since the firmware is the basis for the official GUI block programming model.

As a higher-level abstraction, devices can be grouped into board objects which helps configuring built-in ports and slots and validate externally connected devices.

The library has primarily been tested on Linux. Mac support may be possible, but I have experienced unreliable communication for the USB-Serial bridge on that platform.

## Status
The library is still in development, and has currently only been tested with onboard devices of the Auriga board shipped with the mBot Ranger kit.

## Example usage

Small example setting onboard LEDs and running motors on the Auriga board:

```
#include "boards.hpp"


using namespace libmbot;

int main()
{
    asio::io_context io_context;

    Comm c(io_context, "/dev/ttyUSB0");

    AurigaBoard b(c);

    std::cout << "Setting RGB LEDs to white\n";
    for(uint8_t li=0; li < 12; li++) {
        RGBLedColor white(li+1, RGBColor {100, 100, 100});
        b.m_rgb_led.set_color.request(white);
        usleep(100000);
    }

    PosMotion pm1 {400, 40};
    PosMotion pm2 {-400, 40};
    std::cout << "Positioning motors\n";
    b.m_motor_1.set_pos_motion.request(pm1);
    b.m_motor_2.set_pos_motion.request(pm2);

    usleep(4000000);

    std::cout << "Stopping motors\n";
    b.m_motor_1.set_speed_motion.request(0);
    b.m_motor_2.set_speed_motion.request(0);


    std::cout << "Setting RGB LEDs to black\n";
    RGBLedColor all_black(0, RGBColor {0, 0, 0});
    b.m_rgb_led.set_color.request(all_black);
}
```

## How to install

Libmbot uses CMake as its build system, and can be built standalone or as part of a ROS2 workspace. It depends on ASIO for serial communication, which is automatically downloaded using FetchContent in the CMake file.

To build in ROS2 you can for instance add to your workspace .repo file
```
  libmbot:
    type: git
    url: https://github.com/jtkornel/libmbot
    version: main

```
And then download the sourcecode and build:
```
vcs import src < my_workspace.repos

colcon build --symlink-install
```
## Resources
Useful references for the mBot Arduino firmware
* [Instructables - advanced makeblock sensor DIY](https://www.instructables.com/Advanced-Makeblock-Sensors-DIY/)
* [Makeblock Ranger Arduino Coding Reference](https://docs.google.com/document/d/1EpMWJo9pP2J_pstzXA-XHK8t00Z70SCZYwZ_Kl7VLuw)
