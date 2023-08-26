#pragma once

#include <iostream>
#include <variant>
#include <stdio.h>
#include <unistd.h>
#include <optional>

#include <serial/serial.h>


class Comm
{
    public:
    explicit Comm(serial::Serial &port)
    : m_port(port)
    {
        init_handshake();
    }

    std::vector<uint8_t> write_message(std::vector<uint8_t> msg);

    std::variant<std::string, std::vector<uint8_t>> read_line();

    std::vector<uint8_t> read_message();

    private:

    std::optional<uint8_t> message_index(std::vector<uint8_t> packet) const;
    void init_handshake();
    void flush();

    serial::Serial &m_port;

    serial::Timeout m_msg_timeout {20, 50, 0, 50, 0};
    serial::Timeout m_boot_timeout {serial::Timeout::max(), 2000, 0, 2000, 0};

    uint8_t m_msg_index = 0;
};

