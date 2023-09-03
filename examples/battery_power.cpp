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