#include "comm.hpp"
#include "devices.hpp"

#include <unistd.h>

int main()
{
    asio::io_context io_context;

    Comm c(io_context, "/dev/tty.usbserial-144230");

    auto line = c.read_line();
    std::cout << "Startup msg: " << std::get<std::string>(line) << "\n";

    CommonCmd ss(c, 13);

    usleep(100000);

    while(1)
    {
        float level = ss.battery_power.request(1);
        printf("Battery level %4.2f\n\n", level);
        //usleep(10000);
    }

}