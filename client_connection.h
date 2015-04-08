
#ifndef SFERA_WEBSERVER_CLIENT_CONNECTION_H
#define SFERA_WEBSERVER_CLIENT_CONNECTION_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"

#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_server;

    //------------------------------------------------------------------------------------------------------------------

    class client_connection
    {
    public:
        typedef std::function<void()> transfer_callback_t;

    private:
        const http_server &_server;
        b_ip::tcp::socket *_socket = nullptr;


    public:
        client_connection(const http_server &server, b_ip::tcp::socket *socket);
        ~client_connection();

        void transferAsync(std::ostream &stream, transfer_callback_t callback, size_t maxLength = 0);
        void transferAsync(std::istream &stream, transfer_callback_t callback);

        void close();

    public:
        inline b_ip::tcp::socket& socket()        { return *_socket; }
        inline const http_server& server() const  { return  _server; }
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_CLIENT_CONNECTION_H
