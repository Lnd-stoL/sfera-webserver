
#include "request_processor.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::request_processor::request_processor(sfera_webserver::http_request *req,
                                                      client_connection *connection,
                                                      callback_t callback) :
    _request(req),
    _callback(callback),
    _connection(connection)
{
}


sfera_webserver::request_processor::~request_processor()
{
    delete _request;
    delete _response;
}


void sfera_webserver::request_processor::finalize()
{
    if (!_request->hasHeader("Connection") || _request->headers()["Connection"] != "keep-alive") {
        _connection->close();
        delete _connection;
        _connection = nullptr;
    }
}


void sfera_webserver::request_processor::_createAndSendResponse()
{
    _response = new http_response(*_connection, _determineStatus());
    _response->headers()["Server"] = "sfera-webserver/0.1";
    _response->headers()["Connection"] = "close";

    if (_request->hasHeader("Connection") && _request->headers()["Connection"] == "keep-alive") {
        _response->headers()["Connection"] = "keep-alive";
    }

    _fillResponse();
    _response->sendAsync([this](http_response*) { _handleResponseSent(); });
}
