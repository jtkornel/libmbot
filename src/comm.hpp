#pragma once

#include <iostream>
#include <variant>
#include <stdio.h>
#include <unistd.h>
#include <optional>

#include <serial/serial.h>

#define BUFSIZE 256

// Board -> Device -> Operation -> Data

#if 0
class Message
{
    public:
    std::vector<uint8_t> encode()
    {

         #if 1
        std::vector<uint8_t> msg {0xff, // 0
                      0x55, // 1
                      7, // 2 length
                      0, // 3 index
                      2, // 4 action=RUN
                      62,// 5 device = encoder pid motion
                      2, // 6 port - subcommand SPEED
                      2, // 7 slot - motor num
                      0x2F, // 8 speed LSB
                      0, // 9
                      };
        #else
        std::vector<uint8_t> msg {0xff, // 0
                      0x55, // 1
                      8, // 2 length
                      0, // 3 index
                      2, // 4 action = RUN
                      34,// 5 device = TONE
                      45, // 6 port - BUZZER
                      0, // 7 hz lsb
                      4, // 8 hz msb
                      0xff, // 9 ms lsb
                      1, // 10 ms msb
                      };
        #endif

        return msg;
    }
};
#endif

class Comm
{
    public:
    Comm(serial::Serial &port)
    : m_port(port)
    {
    }

    void flush()
    {
        m_port.flushInput();
    }

    std::vector<uint8_t> write_message(std::vector<uint8_t> msg)
    {
         /*    ff 55 len idx action device port  slot  data a
          *    0  1  2   3   4      5      6     7     8 */

        uint8_t msg_index = m_msg_index++;

        msg[0] = 0xff;
        msg[1] = 0x55;
        msg[2] = msg.size() - 3;
        msg[3] = msg_index;

        std::vector<uint8_t> in_msg;
        size_t retries = 0;
        bool transmitted = false;

        while(!transmitted)
        {
            m_port.write(msg);

            m_port.flushInput();

            in_msg = read_message();

            auto ret_msg_index = message_index(in_msg);

            while(ret_msg_index && !transmitted) {
                if(ret_msg_index == msg_index) {
                    transmitted = true;
                }
                else {
                    in_msg = read_message();
                    ret_msg_index = message_index(in_msg);
                }
            }
    
            retries++;
        }

        return in_msg;
    }

    std::variant<std::string, std::vector<uint8_t>> read_line()
    {
        std::string line;
        
        size_t bytes_read = m_port.readline(line, 128, "\r\n");

        std::vector<uint8_t> packet;



        auto iit = line.begin();
        auto eos = line.end();
        size_t body_len = bytes_read;
        while (iit!=eos && (body_len--))
        {
            packet.emplace_back(*iit++);
        }

        if(bytes_read < 4)
        {
            bytes_read += m_port.read(packet, 4 - bytes_read);
        }

        if(bytes_read < 4)
        {
            printf("******* Received %lu bytes ***************\n", bytes_read);
            return packet;
        }

        uint8_t h0 = packet[0], h1 = packet[1];

        if(h0==0xff && h1==0x55)
        {

            if(bytes_read < 5)
            {
                bytes_read += m_port.read(packet, 5 - bytes_read);
            }

            if(bytes_read < 5) {
                printf("******* Received %lu bytes ***************\n", bytes_read);
                return packet;
            }

            //uint8_t index = packet[2];
            uint8_t typecode = packet[3];
            uint8_t d0 = packet[4];

            size_t num_bytes;
            switch(typecode)
            {
                case 1: // byte
                num_bytes = 1;
                break;

                case 2: // float (double is equivalent on AVR arduino)
                case 5: 
                num_bytes = 4;
                break;

                case 3: // int16_t
                num_bytes = 2;
                break;

                case 4: // string
                num_bytes = 1 + d0; // length and data
                break;

                case 6: // int32_t
                num_bytes = 4;
                break;

                default:
                num_bytes = 0;
                break;
            }

            size_t rem_bytes = (num_bytes + 5) > bytes_read ? num_bytes + 5 - bytes_read : 0;

            if(rem_bytes) {
                m_port.read(packet, rem_bytes);
            }

            return packet;
        }

        return line;
    }

    std::vector<uint8_t> read_message()
    {
        auto line = read_line();

        if(std::holds_alternative<std::string>(line))
        {
            std::cout << std::get<std::string>(line);
            return std::vector<uint8_t> {};
        }
        else
        {
            return std::get<std::vector<uint8_t>>(line);
        }

    }

    private:

    std::optional<uint8_t> message_index(std::vector<uint8_t> packet) const
    {
         /*    ff 55  idx type  data a
          *    0  1   2   3     4     */

         const size_t header_size = 4;
         const size_t tail_size = 2;

        if(packet.size() < header_size + tail_size)
        {
            return std::nullopt;
        }

        return packet[2];
    }

    serial::Serial &m_port;

    uint8_t m_msg_index = 0;
};

