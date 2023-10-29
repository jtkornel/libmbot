#include "comm.hpp"
#include "devices.hpp"

#include <asio.hpp>

#include <chrono>
#include <unistd.h>

using namespace libmbot;

int main()
{
    asio::io_context io_context;

    Comm c(io_context, "/dev/ttyUSB0");

    EncoderBoard motor_1(c, 1);
    EncoderBoard motor_2(c, 2);

    motor_1.set_speed_motion.request(100);
    motor_2.set_speed_motion.request(100);

    while(1)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        auto maybe_speed_1 = motor_1.get_speed.request();

        if(maybe_speed_1)
        {
            printf("Motor 1 speed %4.2f\n", *maybe_speed_1);
        }

        auto maybe_speed_2 = motor_2.get_speed.request();

        if(maybe_speed_2)
        {
            printf("Motor 2 speed %4.2f\n", *maybe_speed_2);
        }

        auto maybe_position_1 = motor_1.get_pos.request();

        if(maybe_position_1)
        {
            printf("Motor 1 position %d\n", *maybe_position_1);
        }

        auto maybe_position_2 = motor_2.get_pos.request();

        if(maybe_position_2)
        {
            printf("Motor 2 position %d\n", *maybe_position_2);
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        std::cout << "Request time " << duration/1000.0f << " ms\n\n";
    }
}