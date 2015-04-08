
#ifndef SFERA_WEBSERVER_REQUEST_H
#define SFERA_WEBSERVER_REQUEST_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include "client_connection.h"

#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_request
    {
    public:
        typedef std::function<void(http_request *)> callback_t;
        enum method { unknown, get, head, post, options, put };

    private:
        callback_t _readyCallback;
        callback_t _timeoutCallback;

        client_connection& _connection;
        b_asio::streambuf _inputBuffer;
        b_asio::deadline_timer _netReadTimer;

        std::unordered_map<std::string, std::string> _headers;
        method _method = method::unknown;

        bool _parsedOK = true;
        std::string _resourceURL;
        std::string _resourcePath;
        std::string _strMethod;


    private:
        void _handleReadUntilNewLine(const b_sys::error_code &err, size_t readBytes);
        void _asyncReadLine();
        void _handleReadLineTimeout(const b_sys::error_code& e);
        void _handleReadLine(const std::string& line);
        void _parseStartLine(const std::string& line);
        void _parseHeaderLine(const std::string &line);
        void _parseURL();
        bool _urlDecode(const std::string& in, std::string& out);

    public:
        http_request(client_connection &connection, callback_t readyCallback, callback_t timeoutCallback);
        bool hasHeader(const std::string& name) const;

    public:
        inline method              reqMethod()    const  { return _method;       }
        inline const std::string&  resourceURL()  const  { return _resourceURL;  }
        inline const std::string&  resourcePath() const  { return _resourcePath; }
        inline bool                parsedOK()     const  { return _parsedOK;     }
        inline client_connection&  connection()   const  { return _connection;   }
        inline decltype(_headers)& headers()             { return _headers;      }
        inline std::string         strMethod()    const  { return _strMethod;    }

    public:
        static http_request* getAsync(client_connection &connection, callback_t readyCallback, callback_t timeoutCallback);
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_REQUEST_H
