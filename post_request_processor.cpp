
#include "post_request_processor.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::post_request_processor::post_request_processor(http_request *req,
                                                                client_connection *connection,
                                                                request_processor::callback_t readyCallback) :
    request_processor(req, connection, readyCallback)
{
    _createAndSendResponse();
}


void sfera_webserver::post_request_processor::_handleResponseSent()
{
//    if (_response->rstatus() == http_response::ok) {
        std::ostringstream *stream = new std::ostringstream();

        size_t contentSize = 0;
        if (_request->hasHeader("Content-Length"))
            contentSize = boost::lexical_cast<size_t>(_request->headers()["Content-Length"]);

        _connection->transferAsync(*stream, [this, stream]() {
            std::cout << "post: " << stream->str() << std::endl;
            _callback(this);

        }, contentSize);
//    }
}


auto sfera_webserver::post_request_processor::_determineStatus() -> http_response::status
{
    return http_response::status::ok;
}


void sfera_webserver::post_request_processor::_fillResponse()
{
    if (_response->isOK()) {
    }
}
