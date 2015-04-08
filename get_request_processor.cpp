
#include "get_request_processor.h"
#include "http_server.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::get_request_processor::get_request_processor(sfera_webserver::http_request *req,
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


void sfera_webserver::get_request_processor::_handleResponseSent()
{
    if (_response->rstatus() == http_response::ok) {
        _requestedFileStream = new std::ifstream(_requestedFileInfo.path().string(),
                                                 std::ios_base::in | std::ios_base::binary);

        _connection->transferAsync(*_requestedFileStream,
                                  [this]() {
            _requestedFileStream->close();
            delete _requestedFileStream;
            _requestedFileStream = nullptr;

            _callback(this);
        });

    } else if (_response->rstatus() == http_response::notFound) {

        auto stream = new std::istringstream(
                _connection->server().standardReplies().pageContentFor404(_request->resourceURL()));

        _connection->transferAsync(*stream,
                                   [this, stream]() {
                                       delete stream;
                                       _callback(this);
                                   });
    }
}


sfera_webserver::http_response::status sfera_webserver::get_request_processor::_determineStatus()
{
    if (!_requestedFileInfo.exists()) {
        return http_response::status::notFound;
    } else {
        return http_response::status::ok;
    }
}


void sfera_webserver::get_request_processor::_fillResponse()
{
    if (_response->isOK()) {

        _response->headers()["Content-Type"]   = _requestedFileInfo.mimeType();
        _response->headers()["Content-Length"] = std::to_string(_requestedFileInfo.size());
    } else {

        _response->headers()["Content-Type"]   = "text/html";
        _response->headers()["Content-Length"] = std::to_string(
                _connection->server().standardReplies().pageContentFor404(_request->resourceURL()).size());
    }
}
