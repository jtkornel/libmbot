# libmbot (work in progress)
C++ library for interfacing with mbot robots.

Libmbot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication over serial port. For now it supports communicating with the standard Makeblock firmware.

As a higher-level abstraction devices can be grouped into board objects which helps configuring built-in ports and slots and validate externally connected devices.
