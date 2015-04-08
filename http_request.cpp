
#include "http_request.h"
#include "http_server.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::http_request::http_request(client_connection &connection, callback_t readyCallback,
                                            callback_t timeoutCallback) :
    _connection(connection),
    _readyCallback(readyCallback),
    _timeoutCallback(timeoutCallback),
    _netReadTimer(const_cast<b_asio::io_service&>(connection.server().ioService()))
{
    _asyncReadLine();
}


void sfera_webserver::http_request::_handleReadUntilNewLine(const b_sys::error_code &err, size_t readBytes)
{
    if (err) {
        return;
    }
    //_netReadTimer.expires_from_now(boost::posix_time::minutes(60));

    b_asio::streambuf::const_buffers_type bufs = _inputBuffer.data();
    std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + readBytes);
    _inputBuffer.consume(readBytes);

    _handleReadLine(str);
}


void sfera_webserver::http_request::_asyncReadLine()
{
    _netReadTimer.expires_from_now(boost::posix_time::seconds(10));
    _netReadTimer.async_wait(boost::bind(&http_request::_handleReadLineTimeout, this, b_asio::placeholders::error));

    b_asio::async_read_until(_connection.socket(), _inputBuffer, "\n",
                             boost::bind(&http_request::_handleReadUntilNewLine, this,
                                         b_asio::placeholders::error, b_asio::placeholders::bytes_transferred));
}


void sfera_webserver::http_request::_handleReadLine(const std::string& line)
{
    if (_method == method::unknown) {
        _parseStartLine(line);
        _asyncReadLine();

    } else {
        if (line[0] != '\n' && line[0] != '\r') {
            _parseHeaderLine(line);
            _asyncReadLine();

        } else {
            _readyCallback(this);
        }
    }
}


void sfera_webserver::http_request::_parseStartLine(const std::string &line)
{
    auto spacePos = line.find(' ');
    _strMethod = line.substr(0, spacePos);
    auto secondSpacePos = line.find(' ', spacePos+1);
    _resourceURL = line.substr(spacePos+1, secondSpacePos - spacePos - 1);
    std::string httpVer = line.substr(secondSpacePos+1);

    boost::trim(_strMethod);
    boost::trim(_resourceURL);
    boost::trim_right_if(httpVer, boost::is_any_of("\r\n"));

    _parseURL();

    if (_strMethod == "GET")      _method = method::get;
    if (_strMethod == "HEAD")     _method = method::head;
    if (_strMethod == "POST")     _method = method::post;
    if (_strMethod == "PUT")      _method = method::put;
    if (_strMethod == "OPTIONS")  _method = method::options;
}


void sfera_webserver::http_request::_parseHeaderLine(const std::string &line)
{
    auto sepPos = std::find(line.begin(), line.end(), ':');
    std::string name(line.begin(), sepPos);
    std::string val(sepPos+1, line.end());

    boost::trim(name);
    boost::trim(val);
    boost::trim_right_if(val, boost::is_any_of("\r\n"));

    _headers[name] = val;
}


auto sfera_webserver::http_request::getAsync(client_connection &connection,
                                             callback_t readyCallback,
                                             callback_t timeoutCallback) -> http_request*
{
    return new http_request(connection, readyCallback, timeoutCallback);
}


bool sfera_webserver::http_request::_urlDecode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {

                int value;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }

            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }

    return true;
}


void sfera_webserver::http_request::_parseURL()
{
    std::string decodedURL;
    if (!_urlDecode(_resourceURL, decodedURL)) {
        _parsedOK = false;
        return;
    }

    auto paramsStartPos = std::find(decodedURL.begin(), decodedURL.end(), '?');
    _resourcePath = std::string(decodedURL.begin(), paramsStartPos);
}


bool sfera_webserver::http_request::hasHeader(const std::string &name) const
{
    return _headers.find(name) != _headers.end();
}


void sfera_webserver::http_request::_handleReadLineTimeout(const b_sys::error_code& e)
{
    if (e.value() != 0) {
        //std::cout << e.message() << std::endl;
        return;
    }

    _connection.socket().cancel();

    _parsedOK = false;
    _timeoutCallback(this);
}
