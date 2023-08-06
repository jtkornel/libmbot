#pragma once

#include "comm.hpp"
#include "operations.hpp"


class CommonCmd
{
    public:
    CommonCmd(Comm& comm, uint8_t port)
    : battery_power(comm, 60, port, SubOpId(0x70), 6)
    {

    }

    GetOperation<float> battery_power;
};

class TemperatureSensor
{
    public:
    TemperatureSensor(Comm& comm, uint8_t port)
    : temperature(comm, 27, port)
    {
    }

    GetOperation<float> temperature;
};

class LightSensor
{
    public:
    LightSensor(Comm& comm, uint8_t port)
    : get_lightlevel(comm, 3, port)
    {
    }

    GetOperation<float> get_lightlevel;
};

class SoundSensor
{
    public:
    SoundSensor(Comm& comm, uint8_t port)
    : get_soundlevel(comm, 7, port)
    {
    }

    GetOperation<float> get_soundlevel;
};

struct PosMotion
{
    int32_t pos;
    int16_t speed;
};

class EncoderBoard
{
    public:
    EncoderBoard(Comm& comm, uint8_t port)
    : get_pos           (comm, 61, port, SubOpId(1))
    , get_speed         (comm, 61, port, SubOpId(2))
    , set_pos_motion    (comm, 62, port, SubOpId(1), 6)
    , set_speed_motion  (comm, 62, port, SubOpId(2), 6)
    , set_pwm_motion    (comm, 62, port, SubOpId(3), 6)
    , set_cur_pos_zero  (comm, 62, port, SubOpId(4), 6)
    , set_car_pos_motion(comm, 62, port, SubOpId(5), 6)
    {
    }

    GetOperation<uint32_t, SubOpId> get_pos;
    GetOperation<float, SubOpId> get_speed;
    SetOperation<PosMotion> set_pos_motion;
    SetOperation<int16_t> set_speed_motion;
    SetOperation<int16_t> set_pwm_motion;
    SetOperation<Noparam> set_cur_pos_zero;
    SetOperation<PosMotion> set_car_pos_motion;
};