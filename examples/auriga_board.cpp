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