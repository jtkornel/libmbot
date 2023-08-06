#pragma once

#ifndef ASIO_STANDALONE
#error "Code assumes non-boost ASIO"
#endif

#include <iostream>
#include <variant>
#include <stdio.h>

#include <asio.hpp>

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
    Comm(asio::io_context &io_context, std::string device_name)
    : m_port(io_context, device_name)
    {
        m_port.set_option(asio::serial_port_base::baud_rate(115200));
        m_port.set_option(asio::serial_port_base::character_size(8 /* data bits */));
        m_port.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        m_port.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        m_port.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
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

        auto buf = asio::buffer(msg);
        std::vector<uint8_t> in_msg;

        while(in_msg.size() == 0)
        {
            size_t bytes_written = asio::write(m_port, buf);

            if(bytes_written != msg.size())
            {
                printf("Write fail\n");
            }

            in_msg = read_message();
        }

         /*    ff 55  idx type  data a
          *    0  1   2   3     4     */
        uint8_t msg_index_read = in_msg[2];
        if(msg_index_read != msg_index)
        {
            printf("Received msg_index %lu differs from sent %lu\n", (size_t)msg_index_read, (size_t)msg_index);
        }

        return in_msg;
    }

    std::variant<std::string, std::vector<uint8_t>> read_line()
    {
        std::error_code cd;

        size_t bytes_read = asio::read_until(m_port, m_b, "\r\n", cd);

        if(cd)
        {
            return "Reading serial port failed";
        }

        std::istream is(&m_b);

        if(bytes_read < 4) {
            std::string line;
            std::getline(is, line);
            return line;
        }

        std::vector<uint8_t> packet;

        auto iit = std::istreambuf_iterator<char>(is);
        auto eos = std::istreambuf_iterator<char>();
        size_t body_len = bytes_read;
        while (iit!=eos && (body_len--)) packet.emplace_back(*iit++);

        uint8_t h0 = packet[0], h1 = packet[1];

        if(h0==0xff && h1==0x55)
        {

            if(bytes_read < 6) {
                return "";
            }

            uint8_t index, typecode, d0;

            index = packet[2];
            typecode = packet[3];
            d0 = packet[4];

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

            printf("Index %u, Typecode %u, num bytes %lu, rem_bytes %lu\n", index, typecode, num_bytes, rem_bytes);

            asio::read(m_port, m_b, asio::transfer_exactly(rem_bytes));
            
            while (iit!=eos && (rem_bytes--)) packet.emplace_back(*iit++);

            return packet;
        }

        return "No packet";
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

    void read_dump_loop()
    {
        while(true) {
        uint8_t data[BUFSIZE];
        size_t n = m_port.read_some(asio::buffer(data, BUFSIZE));
        
        if(n)
        {
            for(size_t m=0; m < n; m++)
            {
                printf("%02x ", data[m]);
            }
            printf("\n");
        }

        }
    }

    private:
    asio::serial_port m_port;
    asio::streambuf m_b {256};

    uint8_t m_msg_index = 0;
};

