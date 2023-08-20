# libmbot (work in progress)
c++ library to communicate with mbot hardware 

Libmot has an object representation of devices connected the Makeblock Arduino boards, with set and get operations that encapsulate communication over serial port. For now it supports communicating with the standard Makeblock firmware.

In addition the devices can be grouped into board objects which helps configuring built-in ports and slot and validate externally connected devices.
