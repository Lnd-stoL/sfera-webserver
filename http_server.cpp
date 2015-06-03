
#include "http_server.h"

#include "get_request_processor.h"
#include "head_request_processor.h"
#include "post_request_processor.h"
#include "logger.hpp"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::http_server::http_server(const http_server_config &&config, b_asio::io_service *ioservice) :
    _config(config),
    _ioservice(ioservice)
{ }


void sfera_webserver::http_server::run()
{
    _ioservice->run();
}


void sfera_webserver::http_server::_handleRequestReady(http_request *req, client_connection *connection)
{
    BOOST_LOG(g_logger) << "got request: " << req->strMethod() << " " << req->resourceURL();

    auto connectionFinalizer = [this](request_processor *rp) {
        rp->finalize();
        auto connection = rp->connection();
        delete rp;

        if (connection) _startAsyncRequestReading(connection);
    };

    if (req->reqMethod() == http_request::method::get) {
        request_processor::processAsync<get_request_processor>(req, connection, connectionFinalizer);
    } if (req->reqMethod() == http_request::method::head) {
        request_processor::processAsync<head_request_processor>(req, connection, connectionFinalizer);
    } if (req->reqMethod() == http_request::method::post) {
        request_processor::processAsync<post_request_processor>(req, connection, connectionFinalizer);
    }
}


void sfera_webserver::http_server::_startAsyncRequestReading(sfera_webserver::client_connection *connection)
{
    http_request::getAsync(*connection,
                           [this, connection](http_request *r) {
                               this->_handleRequestReady(r, connection);
                           },
                           [this, connection](http_request *r) {
                               BOOST_LOG(g_logger) << "connection terminated: timeout";
                               connection->close();
                               delete connection;
                           }
    );
}


void sfera_webserver::http_server::handleClientConnection(b_ip::tcp::socket *acceptedSocket)
{
    auto connection = new client_connection(*this, acceptedSocket);
    _startAsyncRequestReading(connection);
}
