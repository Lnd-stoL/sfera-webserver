
#include "client_connection.h"
#include "logger.hpp"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::client_connection::client_connection(sfera_webserver::http_server const &server,
                                                      b_ip::tcp::socket *socket) :
    _server(server),
    _socket(socket)
{
}


void sfera_webserver::client_connection::transferAsync(std::ostream &stream,
                                                       transfer_callback_t callback, size_t maxLength)
{
    static const size_t BUFFER_SIZE = 4 * 1024;
    static char buffer[BUFFER_SIZE];

    size_t readRequestSize = std::min(BUFFER_SIZE, maxLength - stream.tellp());
    if (maxLength == 0) readRequestSize = BUFFER_SIZE;

    auto bbuff = b_asio::buffer(buffer, BUFFER_SIZE);
    b_asio::async_read(socket(), bbuff, b_asio::transfer_exactly(readRequestSize),
                       [this, &stream, callback, maxLength](const b_sys::error_code &ec, size_t readBytes) {
                           stream.write(buffer, readBytes);

                           if (ec == b_asio::error::eof ||
                               ec == b_asio::error::connection_reset ||
                               (maxLength > 0 && stream.tellp() == maxLength))
                           {
                               callback();
                               return;
                           }

                           transferAsync(stream, callback, maxLength);
                       });
}


void sfera_webserver::client_connection::transferAsync(std::istream &stream, transfer_callback_t callback)
{
    static const int BUFFER_SIZE = 1024 * 1024;
    static char buffer[BUFFER_SIZE];
    long readBytes = stream.read(buffer, BUFFER_SIZE).gcount();

    if (readBytes == 0) {
        callback();
        return;
    }

    auto bbuff = b_asio::buffer(buffer, readBytes);
    b_asio::async_write(socket(), bbuff, b_asio::transfer_exactly(readBytes),
                        [this, &stream, callback](const b_sys::error_code &ec, size_t writtenBytes) {
                            transferAsync(stream, callback);
                        });
}


void sfera_webserver::client_connection::close()
{
    auto remoteEndpoint = _socket->remote_endpoint();
    std::cout << remoteEndpoint.address().to_string() << ":"
                 << remoteEndpoint.port();

    _socket->shutdown(b_ip::tcp::socket::shutdown_type::shutdown_both);
    _socket->close();

    BOOST_LOG(g_logger) << "connection closed by server";
}


sfera_webserver::client_connection::~client_connection()
{
    delete _socket;
}
