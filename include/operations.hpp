#pragma once

#include "comm.hpp"

#include <stdint.h>

#include <tuple>
#include <optional>

namespace libmbot {

using Noparam = std::tuple<>;
using SubOpId = uint8_t;

template<typename D, typename P = Noparam>
class GetOperation
{
    public:
    std::optional<D> request();
    std::optional<D> request(uint8_t key);

    GetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    {
    }

    GetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port,
                 uint8_t slot)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    , m_slot(slot)
    {
    }

    GetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port,
                 uint8_t slot,
                 P param)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    , m_slot(slot)
    , m_param(param)
    {
    }

    private:
    std::vector<uint8_t> create_header(uint8_t slot) const;

    Comm & m_comm;
    uint8_t m_device_id;
    uint8_t m_port;
    std::optional<uint8_t> m_slot = std::nullopt;

    std::optional<P> m_param = std::nullopt;
};

template<typename D, typename P>
inline std::vector<uint8_t> GetOperation<D,P>::create_header(uint8_t slot) const
{
    std::vector<uint8_t> msg {0xff, // 0
                    0x55, // 1
                    0, // 2 length
                    0, // 3 index
                    1, // 4 action=GET
                    m_device_id, // 5 device
                    m_port, // 6 port/subcommand
                    slot, // 7 slot/subcommand
    };

    return msg;
}

template<typename T>
inline void append_message_data(std::vector<uint8_t> &msg, T &data)
{
    size_t msg_size = msg.size();
    uint8_t const* data_ptr = reinterpret_cast<uint8_t const*>(&data);
    size_t data_size = sizeof(data);

    msg.resize(msg_size + data_size);

    memcpy(&msg[msg_size], data_ptr, data_size);
}

template<>
inline void append_message_data<Noparam>(std::vector<uint8_t> &msg, Noparam &data)
{
    (void)msg;
    (void)data;
}

template<typename D>
std::optional<D> parse_message(std::vector<uint8_t> msg, size_t header_size)
{
    size_t msg_size = msg.size();
    const size_t newline_size = 2;
    size_t overhead_size = newline_size + header_size;
    size_t data_size = msg_size >= overhead_size? msg_size - overhead_size: 0;

    if(data_size == sizeof(D)) {
        D result{};
        uint8_t * data_ptr = &msg[header_size];
        uint8_t * result_data_ptr = reinterpret_cast<uint8_t *>(&result);
        memcpy(result_data_ptr, data_ptr, sizeof(D));
        return result;
    }

    return std::nullopt;
}

template<typename D, typename P>
inline std::optional<D> GetOperation<D, P>::request()
{
    auto msg = create_header(m_slot.value_or(0));
    if(m_param) {
        append_message_data<P>(msg, m_param.value());
    }

    std::vector<uint8_t> reply_msg = m_comm.write_message(msg);

    return parse_message<D>(reply_msg, 4); // received header is 4 bytes
}

template<typename D, typename P>
inline std::optional<D> GetOperation<D, P>::request(uint8_t key)
{
    auto msg = create_header(key);
    if(m_param) {
        append_message_data<P>(msg, m_param.value());
    }

    append_message_data<P>(msg, m_param);

    std::vector<uint8_t> reply_msg = m_comm.write_message(msg);

    return parse_message<D>(reply_msg, 4); // received header is 4 bytes
}

inline void start_operation(Comm& comm)
{
    std::vector<uint8_t> msg {0xff, // 0
                    0x55, // 1
                    0, // 2 length
                    0, // 3 index
                    5, // 4 action=START
                    1, // 5 device
    };
    comm.write_message(msg);
}

template <typename D>
class SetOperation
{
    public:
    void request(D data);


    SetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    {
    }

    SetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port,
                 uint8_t slot)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    , m_slot(slot)
    {
    }

    SetOperation(Comm& comm,
                 uint8_t device_id,
                 uint8_t port,
                 uint8_t slot,
                 uint8_t aux_param)
    : m_comm(comm)
    , m_device_id(device_id)
    , m_port(port)
    , m_slot(slot)
    , m_aux_param(aux_param)
    {
    }

    private:
    std::vector<uint8_t> create_header() const;

    Comm& m_comm;

    uint8_t m_device_id;
    uint8_t m_port;
    std::optional<uint8_t> m_slot = std::nullopt;
    std::optional<uint8_t> m_aux_param = std::nullopt;
};

template<typename D>
inline std::vector<uint8_t> SetOperation<D>::create_header() const
{
    std::vector<uint8_t> msg {0xff, // 0
                    0x55, // 1
                    0, // 2 length
                    0, // 3 index
                    2, // 4 action=RUN
                    m_device_id, // 5 device
                    m_port // 6 port/subcommand
    };

    // If slot is set, header length is 8 bytes
    if(m_slot) {
        msg.push_back(m_slot.value());
    }

    // If aux param is set, header length is 9 bytes
    if(m_slot && m_aux_param) {
        msg.push_back(m_aux_param.value());
    }

    return msg;
}


template<typename D>
inline void SetOperation<D>::request(D data)
{
    std::vector<uint8_t> msg = create_header();

    append_message_data(msg, data);

    m_comm.write_message(msg);
}

} // namespace libmbot