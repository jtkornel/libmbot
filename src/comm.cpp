#ifndef ASIO_STANDALONE
#error "Code assumes non-boost ASIO"
#endif

#include <asio.hpp>

int main()
{
    asio::io_context io_context;
}