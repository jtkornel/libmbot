#include "boards.hpp"

using namespace libmbot;

int main()
{
    asio::io_context io_context;

    Comm c(io_context, "/dev/ttyUSB0");

    AurigaBoard b(c);

    b.m_motor_1.set_speed_motion.request(100);

    usleep(1000000);

    b.m_motor_1.set_speed_motion.request(0);
}