
#ifndef SFERA_WEBSERVER_HTTP_SERVER_H
#define SFERA_WEBSERVER_HTTP_SERVER_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include <string>
#include <unordered_set>

#include "http_request.h"
#include "http_response.h"
#include "http_standard_replies.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_server
    {
    private:
        std::string _localHomeDir;
        http_standard_replies _standardReplies;

        b_asio::io_service _ioservice;
        b_ip::tcp::socket *_acceptedSocket;
        b_ip::tcp::acceptor _acceptor;


    private:
        void _startAsyncAccept();
        void _handleAccept(const b_sys::error_code &ec);
        void _handleRequestReady(http_request *req, client_connection *connection);
        void _startAsyncRequestReading(client_connection* connection);

    public:
        http_server(const std::string &homeDir, const std::string &ip, int port);
        ~http_server();

        void run();

    public:
        inline const std::string& localHomeDir() const  { return _localHomeDir; }
        inline const http_standard_replies& standardReplies() const  { return _standardReplies; }
        inline const b_asio::io_service&  ioService() const  { return _ioservice; }
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HTTP_SERVER_H
