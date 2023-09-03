#include "comm.hpp"
#include "devices.hpp"

namespace libmbot {

class AurigaBoard
{
    public:

    AurigaBoard(Comm& comm)
    : m_sound_sensor(comm, 14, 2)
    , m_light_sensor_1(comm, 12, 2)
    , m_light_sensor_2(comm, 11, 2)
    , m_onboard_thermometer(comm, 13, 2)
    , m_motor_1(comm, 1)
    , m_motor_2(comm, 2)
    , m_rgb_led(comm)
    , m_common_cmd(comm)
    , m_gyro(comm, 1)
    {

    }

    SoundSensor m_sound_sensor;
    LightSensor m_light_sensor_1;
    LightSensor m_light_sensor_2;
    AnalogInput m_onboard_thermometer;
    EncoderBoard m_motor_1;
    EncoderBoard m_motor_2;
    RGBLedAuriga m_rgb_led;
    CommonCmd m_common_cmd;
    Gyro m_gyro;
};

} // namespace libmbot