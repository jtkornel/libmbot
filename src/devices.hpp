#pragma once

#include "comm.hpp"
#include "operations.hpp"

class UltrasonicSensor
{
    public:
    UltrasonicSensor(Comm& comm, uint8_t port, uint8_t slot)
    : get_distance_cm(comm, 1, port, slot)
    {
    }

    GetOperation<float> get_distance_cm;
};

class TemperatureSensor
{
    public:
    TemperatureSensor(Comm& comm, uint8_t port, uint8_t slot)
    : temperature(comm, 27, port, slot)
    {
    }

    GetOperation<float> temperature;
};

class LightSensor
{
    public:
    LightSensor(Comm& comm, uint8_t port, uint8_t slot)
    : get_lightlevel(comm, 3, port, slot)
    {
    }

    GetOperation<float> get_lightlevel;
};

class SoundSensor
{
    public:
    SoundSensor(Comm& comm, uint8_t port, uint8_t slot)
    : get_soundlevel(comm, 7, port, slot)
    {
    }

    GetOperation<float> get_soundlevel;
};

class Potentiometer
{
    public:
    Potentiometer(Comm& comm, uint8_t port, uint8_t slot)
    : get_value(comm, 4, port, slot)
    {
    }

    GetOperation<float> get_value;
};

class Joystick
{
    public:
    Joystick(Comm& comm, uint8_t port)
    : get_x(comm, 5, port, 1)
    , get_y(comm, 5, port, 2)
    {
    }

    GetOperation<float> get_x;
    GetOperation<float> get_y;
};

class InfraredSensor
{
    public:
    InfraredSensor(Comm& comm, uint8_t port)
    : get_code(comm, 16, port, 0)
    {
    }

    GetOperation<float> get_code;
};

class PirMotionSensor
{
    public:
    PirMotionSensor(Comm& comm, uint8_t port)
    : get_motion(comm, 15, port, 0)
    {
    }

    GetOperation<float> get_motion;
};

class LinefollowerSensor
{
    public:
    LinefollowerSensor(Comm& comm, uint8_t port)
    : get_code(comm, 17, port, 0)
    {
    }

    GetOperation<float> get_code;
};

class LimitSwitch
{
    public:
    LimitSwitch(Comm& comm, uint8_t port, uint8_t slot)
    : get_code(comm, 21, port, slot)
    {
    }

    GetOperation<float> get_code;
};

class CompassSensor
{
    public:
    CompassSensor(Comm& comm, uint8_t port)
    : get_angle(comm, 26, port, 0)
    {
    }

    GetOperation<float> get_angle;
};

class HumitureSensor
{
    public:
    HumitureSensor(Comm& comm, uint8_t port)
    : get_humidity   (comm, 23, port, 0)
    , get_temperature(comm, 23, port, 1)
    {
    }

    GetOperation<float> get_humidity;
    GetOperation<float> get_temperature;
};

class FlameSensor
{
    public:
    FlameSensor(Comm& comm, uint8_t port)
    : get_value(comm, 24, port, 0)
    {
    }

    GetOperation<float> get_value;
};

class GasSensor
{
    public:
    GasSensor(Comm& comm, uint8_t port)
    : get_value(comm, 25, port, 0)
    {
    }

    GetOperation<float> get_value;
};

class Gyro
{
    public:
    Gyro(Comm& comm, uint8_t port)
    : get_x(comm, 6, port, 1)
    , get_y(comm, 6, port, 2)
    , get_z(comm, 6, port, 3)
    {
    }

    GetOperation<float> get_x;
    GetOperation<float> get_y;
    GetOperation<float> get_z;
};

class Version
{
    public:
    Version(Comm& comm)
    : get_version(comm, 0, 0, 0)
    {
    }

    GetOperation<std::string> get_version; // TODO: parsing for string data
};

class Digital
{
    public:
    Digital(Comm& comm, uint8_t port)
    : get_value(comm, 30, port, 0)
    {
    }

    GetOperation<float> get_value;
};

class Analog
{
    public:
    Analog(Comm& comm, uint8_t port)
    : get_value(comm, 31, port, 0)
    {
    }

    GetOperation<float> get_value;
};

class PulseIn
{
    public:
    PulseIn(Comm& comm, uint8_t port)
    : get_value(comm, 37, port, 0)
    {
    }

    GetOperation<int32_t> get_value;
};

class UltrasonicArduino
{
    public:
    UltrasonicArduino(Comm& comm, uint8_t port_trig, uint8_t port_echo)
    : get_distance(comm, 36, port_trig, port_echo)
    {
    }

    GetOperation<float> get_distance;
};

class Timer
{
    public:
    Timer(Comm& comm)
    : get_time(comm, 50, 0, 0)
    {
    }

    GetOperation<float> get_time;
};

class TouchSensor
{
    public:
    TouchSensor(Comm& comm, uint8_t port)
    : get_touched(comm, 51, port, 0)
    {
    }

    GetOperation<uint8_t> get_touched;
};

class Button
{
    public:
    Button(Comm& comm, uint8_t port)
    : get_pressed(comm, 22, port)
    {
    }

    GetOperation<uint8_t> get_pressed;
};

class CommonCmd
{
    public:
    CommonCmd(Comm& comm)
    : get_battery_power(comm, 60, SubOpId(0x70), 0) // subop in port position
    , get_auriga_mode  (comm, 60, SubOpId(0x71), 0) // subop in port position
    {
    }

    GetOperation<float> get_battery_power;
    GetOperation<uint8_t> get_auriga_mode;
};

class Pm25Sensor
{
    public:
    Pm25Sensor(Comm& comm, uint8_t port)
    : get_pm1_0(comm, 63, port, SubOpId(1))
    , get_pm2_5(comm, 63, port, SubOpId(2))
    , get_pm10 (comm, 63, port, SubOpId(3))
    {
    }

    GetOperation<int16_t> get_pm1_0;
    GetOperation<int16_t> get_pm2_5;
    GetOperation<int16_t> get_pm10;
};

class SmartServo
{
    public:
    SmartServo(Comm& comm, uint8_t port, uint8_t servo_num)
    : get_speed      (comm, 64, SubOpId(0x09), port, servo_num)  // port and slot swapped
    , get_temperature(comm, 64, SubOpId(0x0a), port, servo_num)  // port and slot swapped
    , get_current    (comm, 64, SubOpId(0x0b), port, servo_num)  // port and slot swapped
    , get_voltage    (comm, 64, SubOpId(0x0c), port, servo_num)  // port and slot swapped
    , get_angle      (comm, 64, SubOpId(0x0d), port, servo_num)  // port and slot swapped
    {
    }

    GetOperation<float, uint8_t> get_speed;
    GetOperation<float, uint8_t> get_temperature;
    GetOperation<float, uint8_t> get_current;
    GetOperation<float, uint8_t> get_voltage;
    GetOperation<float, uint8_t> get_angle;
};

struct PosMotion
{
    int32_t pos;
    int16_t speed;
};

class EncoderBoard
{
    public:
    EncoderBoard(Comm& comm, uint8_t slot)
    : get_pos           (comm, 61, 0,    slot, SubOpId(1))
    , get_speed         (comm, 61, 0,    slot, SubOpId(2))
    , set_pos_motion    (comm, 62, SubOpId(1), 0)
    , set_speed_motion  (comm, 62, SubOpId(2), 0)
    , set_pwm_motion    (comm, 62, SubOpId(3), 0)
    , set_cur_pos_zero  (comm, 62, SubOpId(4), 0)
    , set_car_pos_motion(comm, 62, SubOpId(5), 0)
    , set_target_pwm    (comm, 61, slot)          // Redundant with set_pwm_motion
    {
    }

    GetOperation<uint32_t, SubOpId> get_pos;
    GetOperation<float, SubOpId> get_speed;
    SetOperation<PosMotion> set_pos_motion;
    SetOperation<int16_t> set_speed_motion;
    SetOperation<int16_t> set_pwm_motion;
    SetOperation<Noparam> set_cur_pos_zero;
    SetOperation<PosMotion> set_car_pos_motion;
    SetOperation<int16_t> set_target_pwm;
};

class DCMotor
{
    public:
    DCMotor(Comm& comm, uint8_t port)
    : set_speed(comm, 10, port)
    {
    }

    SetOperation<int16_t> set_speed;
};

