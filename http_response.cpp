
#include "http_response.h"
#include "http_server.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::http_response::http_response(client_connection &connection, status res) :
    _connection(connection),
    _outputStream(&_outputBuffer),
    _status(res)
{
    _outputStream << "HTTP/1.0 " << res << " ";
    _outputStream << connection.server().standardReplies().shortDescriptionFor(_status) << "\r\n";
}


void sfera_webserver::http_response::sendAsync(ready_callback_t readyCallback)
{
    _writeHeaders();
    _outputStream << "\r\n";
    _outputStream.flush();   // to _outputBuffer

    b_asio::async_write(_connection.socket(), _outputBuffer, b_asio::transfer_exactly(_outputBuffer.size()),
                        [this, readyCallback](const b_sys::error_code &ec, size_t writtenBytes) {
                            _outputBuffer.consume(writtenBytes);
                            readyCallback(this);
                        });
}


void sfera_webserver::http_response::_writeHeaders()
{
    for (auto entry : _headers){
        _outputStream << entry.first << ": " << entry.second << "\r\n";
    }
}
