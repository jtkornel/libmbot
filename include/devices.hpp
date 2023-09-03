#pragma once

#include "comm.hpp"
#include "operations.hpp"

namespace libmbot {
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
    LightSensor(Comm& comm, uint8_t port)
    : get_light_level(comm, 3, port)
    , set_led_light_level(comm, 3, port)
    {
    }

    GetOperation<float> get_light_level;
    SetOperation<uint8_t> set_led_light_level;
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
    explicit Version(Comm& comm)
    : get_version(comm, 0, 0, 0)
    {
    }

    GetOperation<std::string> get_version; // TODO: parsing for string data
};

class DigitalInput
{
    public:
    DigitalInput(Comm& comm, uint8_t port)
    : get_value(comm, 30, port, 0)
    {
    }

    GetOperation<float> get_value;
};

class AnalogInput
{
    public:
    AnalogInput(Comm& comm, uint8_t port, uint8_t slot)
    : get_value(comm, 31, port, slot)
    {
    }

    GetOperation<float> get_value;
};

class PulseInput
{
    public:
    PulseInput(Comm& comm, uint8_t port)
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
    explicit Timer(Comm& comm)
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
    explicit CommonCmd(Comm& comm)
    : get_battery_power(comm, 60, SubOpId(0x70), 0) // subop in port position
    , get_auriga_mode  (comm, 60, SubOpId(0x71), 0) // subop in port position
    , set_auriga_mode  (comm, 60, SubOpId(0x11)) // subop in port position
    {
    }

    GetOperation<float> get_battery_power;
    GetOperation<uint8_t> get_auriga_mode; // TODO: use c++ enumerator type
    SetOperation<uint8_t> set_auriga_mode;
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

struct RGBColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed));

struct SmartServoMove
{
    int32_t pos;
    float speed;
} __attribute__((packed));

class SmartServo
{
    public:
    SmartServo(Comm& comm, uint8_t port, uint8_t servo_num)
    : get_speed      (comm, 64, SubOpId(0x09), port, servo_num)  // port and slot swapped
    , get_temperature(comm, 64, SubOpId(0x0a), port, servo_num)  // port and slot swapped
    , get_current    (comm, 64, SubOpId(0x0b), port, servo_num)  // port and slot swapped
    , get_voltage    (comm, 64, SubOpId(0x0c), port, servo_num)  // port and slot swapped
    , get_angle      (comm, 64, SubOpId(0x0d), port, servo_num)  // port and slot swapped
    , set_break      (comm, 64, SubOpId(0x01), port, servo_num)  // port and slot swapped
    , set_rgb        (comm, 64, SubOpId(0x02), port, servo_num)  // port and slot swapped
    , set_handshake  (comm, 64, SubOpId(0x03), port, servo_num)  // port and slot swapped
    , set_move_to    (comm, 64, SubOpId(0x04), port, servo_num)  // port and slot swapped
    , set_move       (comm, 64, SubOpId(0x05), port, servo_num)  // port and slot swapped
    , set_pwm        (comm, 64, SubOpId(0x06), port, servo_num)  // port and slot swapped
    , set_zero       (comm, 64, SubOpId(0x07), port, servo_num)  // port and slot swapped
    , set_init_angle (comm, 64, SubOpId(0x08), port, servo_num)  // port and slot swapped
    {
    }

    GetOperation<float, uint8_t> get_speed;
    GetOperation<float, uint8_t> get_temperature;
    GetOperation<float, uint8_t> get_current;
    GetOperation<float, uint8_t> get_voltage;
    GetOperation<float, uint8_t> get_angle;
    SetOperation<uint8_t> set_break;
    SetOperation<RGBColor> set_rgb;
    SetOperation<Noparam> set_handshake;
    SetOperation<SmartServoMove> set_move_to;
    SetOperation<SmartServoMove> set_move;
    SetOperation<int16_t> set_pwm;
    SetOperation<Noparam> set_zero;
    SetOperation<Noparam> set_init_angle;
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
    , set_pos_motion    (comm, 62, SubOpId(1), slot)
    , set_speed_motion  (comm, 62, SubOpId(2), slot)
    , set_pwm_motion    (comm, 62, SubOpId(3), slot)
    , set_cur_pos_zero  (comm, 62, SubOpId(4), slot)
    , set_car_pos_motion(comm, 62, SubOpId(5), slot)
    , set_target_pwm    (comm, 61, 0, slot)          // Redundant with set_pwm_motion
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

struct SpeedDistance
{
    int16_t speed;
    int32_t distance;
} __attribute__((packed));

class Stepper
{
    public:
    Stepper(Comm& comm, uint8_t port)
    : set_speed_distance(comm, 40, port)
    {
    }

    SetOperation<SpeedDistance> set_speed_distance;
};



template<size_t N>
struct RGBLedColors
{
    public:
    RGBLedColors(RGBColor colors[N])
    {
        for(size_t i = 0; i < N; ++i) {
            this->colors[i] = colors[i];
        }
    }
    private:
    uint8_t led_index = 1;
    RGBColor colors[N];
} __attribute__((packed));

struct RGBLedColor
{
    public:
    /// @brief Construct a new RGBLedColor object
    /// @param color
    /// Sets all LEDs to the same color
    RGBLedColor(RGBColor color)
    : color(color)
    {
    }

    /// @brief Construct a new RGBLedColor object
    /// @param led_index
    /// @param color
    /// Sets a single LED to a color
    RGBLedColor(uint8_t led_index, RGBColor color)
    : led_index(led_index)
    , color(color)
    {
    }

    private:
    uint8_t led_index=0;
    RGBColor color;
} __attribute__((packed));

class RGBLedAuriga
{
    public:
    RGBLedAuriga(Comm& comm)
    : set_colors(comm, 8, 0, 2)
    , set_color(comm, 8, 0, 2)
    {
    }

    /// @brief Set all LEDs to individual colors
    SetOperation<RGBLedColors<12>> set_colors;
    /// @brief Set a single LED to a specific color
    SetOperation<RGBLedColor> set_color;
};

class RGBLed
{
    public:
    RGBLed(Comm& comm, uint8_t port)
    : set_colors(comm, 8, port, 0)
    , set_color(comm, 8, port, 0)
    {
    }

    SetOperation<RGBLedColors<4>> set_colors;
    SetOperation<RGBLedColor> set_color;
};

class Servo
{
    public:
    Servo(Comm& comm, uint8_t port, uint8_t slot)
    : set_angle(comm, 11, port, slot)
    {
    }

    SetOperation<uint8_t> set_angle; // 0 - 180
};


class SevenSegmentDisplay
{
    public:
    SevenSegmentDisplay(Comm& comm, uint8_t port)
    : set_number(comm, 9, port)
    {
    }

    SetOperation<float> set_number;
};

struct LedBitmapData
{
    public:
    LedBitmapData(uint8_t px, uint8_t py, uint8_t bitmap[16])
    : m_px(px)
    , m_py(py)
    {
        for(size_t i = 0; i < 16; i++) {
            this->m_bitmap[i] = bitmap[i];
        }
    }
    private:
    uint8_t m_action = 2;
    int8_t m_px;
    int8_t m_py;
    uint8_t m_bitmap[16];
} __attribute__((packed));

struct LedClockSetting
{
    public:
    LedClockSetting(int8_t point, int8_t hour, int8_t minute)
    : m_point(point)
    , m_hour(hour)
    , m_minute(minute)
    {
    }
    private:
    uint8_t m_action = 3;
    int8_t m_point;
    int8_t m_hour;
    int8_t m_minute;
} __attribute__((packed));

struct LedFloatNumber
{
    public:
    LedFloatNumber(float number)
    : m_number(number)
    {
    }
    private:
    uint8_t m_action = 4;
    float m_number;
} __attribute__((packed));


class LedMatrix
{
    public:
    LedMatrix(Comm& comm, uint8_t port)
    : set_bitmap_data(comm, 41, port)
    , set_clock_setting(comm, 41, port)
    , set_float_number(comm, 41, port)
    {
    }

    // TODO: add set string operation,
    // this is difficult because of the
    // reliance of POD types when copying
    // to the serial port buffer
    SetOperation<LedBitmapData> set_bitmap_data;
    SetOperation<LedClockSetting> set_clock_setting;
    SetOperation<LedFloatNumber> set_float_number;
};

enum ShutterSetting : uint8_t
{
    SHUTTER_OFF = 0,
    SHUTTER_ON = 1,
    FOCUS_ON = 2,
    FOCUS_OFF = 3
};

class CameraShutter
{
    public:
    CameraShutter(Comm& comm, uint8_t port)
    : set_shutter(comm, 20, port)
    {
    }

    SetOperation<ShutterSetting> set_shutter;
};

class DigitalOutput
{
    public:
    DigitalOutput(Comm& comm, uint8_t port)
    : set_value(comm, 30, port)
    {
    }

    SetOperation<uint8_t> set_value;
};

class PWMOutput
{
    public:
    PWMOutput(Comm& comm, uint8_t port)
    : set_value(comm, 31, port)
    {
    }

    SetOperation<uint8_t> set_value;
};

struct BuzzerTone
{
    public:
    /// @brief Construct a new Buzzer Tone object
    /// @param frequency in Hz
    /// @param duration in ms
    BuzzerTone(int16_t frequency, int16_t duration)
    : m_frequency(frequency)
    , m_duration(duration)
    {
    }
    /// @brief Construct a new Buzzer Tone object to stop sound
    BuzzerTone()
    : m_frequency(0)
    , m_duration(0)
    {
    }
    private:
    int16_t m_frequency;
    int16_t m_duration;
} __attribute__((packed));
class Buzzer
{
    public:
    Buzzer(Comm& comm, uint8_t port)
    : set_tone(comm, 34, port)
    {
    }

    SetOperation<BuzzerTone> set_tone;
};

} // namespace libmbot
