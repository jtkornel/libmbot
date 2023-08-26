#include "comm.hpp"


std::vector<uint8_t> Comm::write_message(std::vector<uint8_t> msg)
{
        /*    ff 55 len idx action device port  slot  data a
        *    0  1  2   3   4      5      6     7     8 */

    uint8_t msg_index = m_msg_index++;

    msg[0] = 0xff;
    msg[1] = 0x55;
    msg[2] = msg.size() - 3;
    msg[3] = msg_index;

    std::vector<uint8_t> return_msg;
    bool return_ok = false;

    // Resend message until a valid response is received
    while(!return_ok) {
        m_port.write(msg);

        m_port.flushInput();

        return_msg = read_message();

        auto return_msg_ind = message_index(return_msg);

        // Ignore possible older unread messages
        while(return_msg_ind && !return_ok) {
            if(return_msg_ind != msg_index) {
                return_msg = read_message();
                return_msg_ind = message_index(return_msg);
            }
            else {
                return_ok = true;
            }
        }
    }

    return return_msg;
}

std::variant<std::string, std::vector<uint8_t>> Comm::read_line()
{
    std::string line;

    size_t bytes_read = m_port.readline(line, 128, "\r\n");

    std::vector<uint8_t> packet;
    auto iit = line.begin();
    auto eos = line.end();
    size_t body_len = bytes_read;
    while (iit!=eos && (body_len--)) {
        packet.emplace_back(*iit++);
    }

    const size_t prefix_bytes = 2;

    if(bytes_read < prefix_bytes) {
        bytes_read += m_port.read(packet, prefix_bytes - bytes_read);
    }

    if(bytes_read < prefix_bytes) {
        printf("******* Received %lu bytes ***************\n", bytes_read);
        return packet;
    }

    uint8_t h0 = packet[0], h1 = packet[1];

    if(h0==0xff && h1==0x55) {

        const size_t header_bytes = 4;
        const size_t d0_bytes = 1;
        const size_t min_bytes = header_bytes + d0_bytes;

        if(bytes_read < min_bytes) {
            bytes_read += m_port.read(packet, min_bytes - bytes_read);
        }

        if(bytes_read < min_bytes) {
            printf("******* Received %lu bytes ***************\n", bytes_read);
            return packet;
        }

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

        const size_t tail_bytes = 2;
        size_t rem_bytes = (num_bytes + header_bytes + tail_bytes) > bytes_read ? num_bytes + header_bytes + tail_bytes - bytes_read : 0;

        if(rem_bytes) {
            m_port.read(packet, rem_bytes);
        }

        return packet;
    }

    return line;
}


std::vector<uint8_t> Comm::read_message()
{
    auto line = read_line();

    if(std::holds_alternative<std::string>(line)) {
        std::cout << std::get<std::string>(line);
        return std::vector<uint8_t> {};
    }
    else {
        return std::get<std::vector<uint8_t>>(line);
    }
}

std::optional<uint8_t> Comm::message_index(std::vector<uint8_t> packet) const
{
    /*    ff 55  idx type  data a
    *    0  1   2   3     4     */

    const size_t header_size = 4;
    const size_t tail_size = 2;

    if(packet.size() < header_size + tail_size) {
        return std::nullopt;
    }

    return packet[2];
}

void Comm::init_handshake()
{
    m_port.setTimeout(m_boot_timeout);
    bool init_data_read = false;

    while(!init_data_read)
    {
        flush();
        auto line = read_line();
        if(std::holds_alternative<std::string>(line)) {
            std::cout << "Startup msg: " << std::get<std::string>(line) << "\n";
            m_port.setTimeout(m_msg_timeout);
        }
        else {
            auto packet = std::get<std::vector<uint8_t>>(line);
            if(packet.size()==0) {
                init_data_read = true;
            }
        }
    }

    m_port.setTimeout(m_msg_timeout);

    usleep(1500000);
    std::cout << "Handshake completed\n";
}

void Comm::flush()
{
    m_port.flushInput();
}
