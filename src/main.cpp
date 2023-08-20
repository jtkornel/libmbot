#include "comm.hpp"
#include "devices.hpp"

#include <unistd.h>

#include <serial/serial.h>

#include <chrono>

int main()
{
    serial::Timeout msg_timeout(20, 50, 0, 50, 0);
    serial::Timeout boot_timeout(serial::Timeout::max(), 2000, 0, 2000, 0);
    serial::Serial ser("/dev/tty.usbserial-143220", 115200, boot_timeout);

    Comm c(ser);

    bool init_data_read = false;

    while(!init_data_read)
    {
        c.flush();
        auto line = c.read_line();
        if(std::holds_alternative<std::string>(line)) {
            std::cout << "Startup msg: " << std::get<std::string>(line) << "\n";
            ser.setTimeout(msg_timeout);
        }
        else {
            auto packet = std::get<std::vector<uint8_t>>(line);
            if(packet.size()==0) {
                init_data_read = true;
            }
        }
    }

    ser.setTimeout(msg_timeout);

    usleep(1500000);
    std::cout << "Handshake completed\n";

    CommonCmd ss(c, 13);

    while(1)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        auto maybe_level = ss.battery_power.request(1);
        if(maybe_level)
        {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

            printf("Battery level %4.2f\n", *maybe_level);
            if(duration > 120000)
            std::cout << "Request time " << duration/1000.0f << "\n\n";
        }
    }

}