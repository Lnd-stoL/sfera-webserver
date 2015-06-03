
#ifndef SFERA_WEBSERVER_HTTP_SERVER_H
#define SFERA_WEBSERVER_HTTP_SERVER_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include <string>
#include <unordered_set>

#include "http_request.h"
#include "http_response.h"
#include "http_standard_replies.h"
#include "http_server_config.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_server
    {
    private:
        http_server_config _config;
        http_standard_replies _standardReplies;

        b_asio::io_service *_ioservice;


    private:
        void _handleRequestReady(http_request *req, client_connection *connection);
        void _startAsyncRequestReading(client_connection* connection);

    public:
        http_server(const http_server_config &&config, b_asio::io_service *ioservice);

        void run();
        void handleClientConnection(b_ip::tcp::socket *acceptedSocket);

    public:
        inline const std::string& localHomeDir() const  { return _config.wwwHomeDir; }
        inline const http_standard_replies& standardReplies() const  { return _standardReplies; }
        inline const b_asio::io_service&  ioService() const  { return *_ioservice; }

        inline b_asio::io_service&  ioService()  { return *_ioservice; }
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HTTP_SERVER_H
