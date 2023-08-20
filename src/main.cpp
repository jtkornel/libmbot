#include "comm.hpp"
#include "devices.hpp"

#include <serial/serial.h>

#include <chrono>
#include <unistd.h>


int main()
{

    serial::Serial ser("/dev/ttyUSB0", 115200);

    Comm c(ser);

    CommonCmd cc(c);

    while(1)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        auto maybe_level = cc.get_battery_power.request();
        if(maybe_level)
        {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

            printf("Battery level %4.2f\n", *maybe_level);
            std::cout << "Request time " << duration/1000.0f << " ms\n\n";
        }
    }
}