#include "comm.hpp"


namespace libmbot {

std::vector<uint8_t> Comm::write_message(std::vector<uint8_t> msg)
{
        /*    ff 55 len idx action device port  slot  data a
        *    0  1  2   3   4      5      6     7     8 */

    uint8_t msg_index = m_msg_index++;

    msg[0] = 0xff;
    msg[1] = 0x55;
    msg[2] = msg.size() - 3;
    msg[3] = msg_index;

    auto buf = asio::buffer(msg);
    std::optional<std::vector<uint8_t>> maybe_msg;
    bool return_ok = false;

    // Resend message until a valid response is received
    while(!return_ok) {
        asio::write(m_port, buf);

        maybe_msg = read_message();

        auto return_msg_ind = message_index(maybe_msg);

        // Ignore possible older unread messages
        while(return_msg_ind && !return_ok) {
            if(return_msg_ind != msg_index) {
                maybe_msg = read_message();
                return_msg_ind = message_index(maybe_msg);
            }
            else {
                return_ok = true;
            }
        }
    }

    return *maybe_msg;
}


void Comm::move_streambuf_to_packet(std::vector<uint8_t>& packet)
{
    std::istream is(&m_streambuf);
    auto iit = std::istreambuf_iterator<char>(is);
    auto eos = std::istreambuf_iterator<char>();
    while (iit!=eos) {
        packet.emplace_back(*iit++);
    }
}

std::variant<std::string, std::vector<uint8_t>> Comm::read_message_or_text()
{
    std::error_code cd;

    size_t bytes_read = asio::read_until(m_port, m_streambuf, "\r\n", cd);

    if(cd) {
        return "Reading serial port failed";
    }

    const size_t prefix_bytes = 2;
    if(bytes_read < prefix_bytes) {
        asio::read(m_port, m_streambuf, asio::transfer_exactly(prefix_bytes - bytes_read));
        bytes_read = prefix_bytes;
    }

    std::vector<uint8_t> packet;
    move_streambuf_to_packet(packet);

    uint8_t h0 = packet[0], h1 = packet[1];

    if(h0==0xff && h1==0x55) {

        const size_t header_bytes = 4;
        const size_t d0_bytes = 1;
        const size_t min_bytes = header_bytes + d0_bytes;

        if(bytes_read < min_bytes) {
            asio::read(m_port, m_streambuf, asio::transfer_exactly(min_bytes - bytes_read));
            move_streambuf_to_packet(packet);
            bytes_read = min_bytes;
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
            asio::read(m_port, m_streambuf, asio::transfer_exactly(rem_bytes));
            move_streambuf_to_packet(packet);
        }

        return packet;
    }

    // Assuming it is a debug text message,
    // copy packet bytes to string
    std::string text;

    for(auto b : packet) {
        text += b;
    }

    return text;
}


std::optional<std::vector<uint8_t>> Comm::read_message()
{
    auto data = read_message_or_text();

    if(std::holds_alternative<std::string>(data)) {
        std::cout << std::get<std::string>(data);
        return std::nullopt;
    }
    else {
        return std::get<std::vector<uint8_t>>(data);
    }
}

std::optional<uint8_t> Comm::message_index(std::optional<std::vector<uint8_t>> maybe_packet) const
{
    if(!maybe_packet) {
        return std::nullopt;
    }

    auto packet = *maybe_packet;

    /*   ff 55  idx type  data a
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
    bool handshake_ok = false;

    while(!handshake_ok)
    {
        auto data = read_message_or_text();
        if(std::holds_alternative<std::string>(data)) {
            std::cout << "Startup msg: " << std::get<std::string>(data) << "\n";
            handshake_ok = true;
        }
        else {
            std::cout << "Received initial binary data:\n";
            for(auto b : std::get<std::vector<uint8_t>>(data)) {
                printf("%02x ", b);
            }
            printf("\n");
        }
    }

    usleep(1500000);
    std::cout << "Handshake completed\n\n";
}

} // namespace libmbot