#pragma once

#include "comm.hpp"

#include <stdint.h>

#include <tuple>


using Noparam = std::tuple<>;
using SubOpId = uint8_t;

template<typename D, typename P = Noparam>
class GetOperation
{
    public:
    D request(uint8_t slot);

    GetOperation(Comm& comm,
                 uint8_t mbot_device_id,
                 uint8_t port)
    : m_comm(comm)
    , m_mbot_device_id(mbot_device_id)
    , m_port(port)
    {
    }

    GetOperation(Comm& comm,
                 uint8_t mbot_device_id,
                 uint8_t port,
                 uint8_t suboperation,
                 uint8_t suboperation_byte)
    : m_comm(comm)
    , m_mbot_device_id(mbot_device_id)
    , m_mbot_suboperation(suboperation)
    , m_suboperation_byte(suboperation_byte)
    , m_port(port)
    {
    }

    GetOperation(Comm& comm,
                 uint8_t mbot_device_id,
                 uint8_t port,
                 P param)
    : m_comm(comm)
    , m_mbot_device_id(mbot_device_id)
    , m_port(port)
    , m_param(param)
    {
    }



    private:
    bool suboperation_set() const
    {
        return m_suboperation_byte != 0;
    }

    std::vector<uint8_t> create_header(uint8_t slot) const;

    Comm & m_comm;
    uint8_t m_mbot_device_id;
    uint8_t m_mbot_suboperation;
    uint8_t m_suboperation_byte; // TODO: optional

    uint8_t m_port;
    uint8_t m_port_byte=6;

    P m_param;
};

template<typename D, typename P>
inline std::vector<uint8_t> GetOperation<D,P>::create_header(uint8_t slot) const
{
    std::vector<uint8_t> msg {0xff, // 0
                    0x55, // 1
                    0, // 2 length
                    0, // 3 index
                    1, // 4 action=GET
                    m_mbot_device_id, // 5 device
                    0, // 6 port/subcommand
                    slot, // 7 slot/subcommand
    };

    size_t header_size = msg.size();

    if(m_port_byte < header_size) {
        msg[m_port_byte] = m_port;
    }

    if(suboperation_set() && m_suboperation_byte < header_size) {
        msg[m_suboperation_byte] = m_mbot_suboperation;
    }

    return msg;
}

template<typename T>
inline void append_message_data(std::vector<uint8_t> msg, T &data)
{
    size_t msg_size = msg.size();
    uint8_t const* data_ptr = reinterpret_cast<uint8_t const*>(&data);
    size_t data_size = sizeof(data);

    msg.resize(msg_size + data_size);

    memcpy(&msg[msg_size], data_ptr, data_size);
}

template<>
inline void append_message_data<Noparam>(std::vector<uint8_t> msg, Noparam &data)
{
    (void)msg;
    (void)data;
}

template<typename D>
D parse_message(std::vector<uint8_t> msg, size_t header_size)
{
    size_t msg_size = msg.size();
    size_t data_size = msg_size >= header_size ? msg_size - header_size : 0; // Newline

    D result{};

    if(data_size == sizeof(D)) {
        uint8_t * data_ptr = &msg[header_size];
        uint8_t * result_data_ptr = reinterpret_cast<uint8_t *>(&result);
        memcpy(result_data_ptr, data_ptr, sizeof(D));
    }

    return result;
}

template<typename D, typename P>
inline D GetOperation<D, P>::request(uint8_t slot)
{
    auto msg = create_header(slot);
    append_message_data<P>(msg, m_param);

    std::vector<uint8_t> reply_msg = m_comm.write_message(msg);

    return parse_message<D>(reply_msg, 4); // received header is 4 bytes
}


template <typename D>
class SetOperation
{
    public:
    void request(uint8_t slot, D data);

    SetOperation(Comm& comm,
                 uint8_t mbot_device_id,
                 uint8_t port)
    : m_comm(comm)
    , m_mbot_device_id(mbot_device_id)
    , m_port(port)
    {
    }

    SetOperation(Comm& comm,
                 uint8_t mbot_device_id,
                 uint8_t port,
                 uint8_t suboperation,
                 uint8_t suboperation_byte)
    : m_comm(comm)
    , m_mbot_device_id(mbot_device_id)
    , m_mbot_suboperation(suboperation)
    , m_suboperation_byte(suboperation_byte)
    , m_port(port)
    {
    }

    private:
    bool suboperation_set()
    {
        return m_suboperation_byte != 0;
    }

    std::vector<uint8_t> create_header(uint8_t slot) const;

    Comm& m_comm;

    uint8_t m_mbot_device_id;
    uint8_t m_mbot_suboperation=0;
    uint8_t m_suboperation_byte=0; // TODO: use optional

    uint8_t m_port;
    uint8_t m_port_byte=6;
};

template<typename D>
inline std::vector<uint8_t> SetOperation<D>::create_header(uint8_t slot) const
{
    std::vector<uint8_t> msg {0xff, // 0
                    0x55, // 1
                    0, // 2 length
                    0, // 3 index
                    2, // 4 action=RUN
                    m_mbot_device_id, // 5 device
                    0, // 6 port/subcommand
                    slot, // 7 slot/subcommand
    };

    size_t header_size = msg.size();

    if(m_port_byte < header_size) {
        msg[m_port_byte] = m_port;
    }

    if(suboperation_set() && m_suboperation_byte < header_size) {
        msg[m_suboperation_byte] = m_mbot_suboperation;
    }

    return msg;
}


template<typename D>
inline void SetOperation<D>::request(uint8_t slot, D data)
{
    std::vector<uint8_t> msg = create_header(slot);

    append_message_data(msg, data);

    m_comm.write_message(msg);
}

