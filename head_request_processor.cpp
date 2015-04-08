
#include "head_request_processor.h"
#include "http_server.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::head_request_processor::head_request_processor(sfera_webserver::http_request *req,
                                                                client_connection *connection,
                                                                callback_t callback) :
    request_processor(req, connection, callback)
{
    b_fs::path requestedPath(_connection->server().localHomeDir());
    requestedPath /= req->resourcePath();
    _requestedFileInfo = file_info(requestedPath);

    if (_requestedFileInfo.isDirectory()) {
        _requestedFileInfo = file_info(requestedPath / "index.html");
    }

    _createAndSendResponse();
}


void sfera_webserver::head_request_processor::_handleResponseSent()
{
    _callback(this);
}


auto sfera_webserver::head_request_processor::_determineStatus() -> http_response::status
{
    if (!_requestedFileInfo.exists()) {
        return http_response::status::notFound;
    } else {
        return http_response::status::ok;
    }
}


void sfera_webserver::head_request_processor::_fillResponse()
{
    if (_response->isOK()) {

        _response->headers()["Content-Length"] = std::to_string(_requestedFileInfo.size());
        _response->headers()["Content-Type"] = _requestedFileInfo.mimeType();
    }
}
