
#ifndef SFERA_WEBSERVER_HTTP_RESPONSE_H
#define SFERA_WEBSERVER_HTTP_RESPONSE_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include "client_connection.h"

#include <string>
#include <unordered_map>
#include <fstream>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_response
    {
    public:
        typedef std::function<void(http_response*)> ready_callback_t;
        enum status { unknown, ok = 200, notFound = 404 };

    private:
        client_connection& _connection;
        b_asio::streambuf _outputBuffer;
        std::ostream _outputStream;

        status _status = status::unknown;
        std::unordered_map<std::string, std::string> _headers;


    private:
        void _writeHeaders();

    public:
        http_response(client_connection &connection, status res);
        void sendAsync(ready_callback_t readyCallback);

    public:
        inline decltype(_headers)& headers()  { return _headers; }
        inline std::ostream&       stream()   { return _outputStream; }

        inline status rstatus() const  { return _status; }
        inline bool   isOK() const     { return _status == status::ok; }
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HTTP_RESPONSE_H
