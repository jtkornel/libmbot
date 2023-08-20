# libmbot (work in progress)
c++ library to communicate with mbot hardware.

Libmbot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication over serial port. For now it supports communicating with the standard Makeblock firmware.

As a higher-level abstraction the devices can be grouped into board objects which helps configuring built-in ports and slot and validate externally connected devices.
