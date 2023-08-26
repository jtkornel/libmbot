#pragma once

#ifndef ASIO_STANDALONE
#error "Code assumes non-boost ASIO"
#endif

#include <iostream>
#include <variant>
#include <stdio.h>
#include <unistd.h>
#include <optional>

#include <asio.hpp>


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
        init_handshake();
    }

    std::vector<uint8_t> write_message(std::vector<uint8_t> msg);

    std::variant<std::string, std::vector<uint8_t>> read_line();

    std::vector<uint8_t> read_message();

    private:

    std::optional<uint8_t> message_index(std::vector<uint8_t> packet) const;
    void init_handshake();

    asio::serial_port m_port;
    asio::streambuf m_b {256};

    uint8_t m_msg_index = 0;
};

