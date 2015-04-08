
#include "http_server.h"

#include "get_request_processor.h"
#include "head_request_processor.h"
#include "post_request_processor.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::http_server::http_server(const std::string &homeDir, const std::string &ip, int port) :
    _localHomeDir(homeDir),
    _acceptedSocket(nullptr),
    _acceptor(_ioservice, b_ip::tcp::endpoint(b_ip::address::from_string(ip), (unsigned short)port), true)
{
    _acceptor.listen();
    _startAsyncAccept();

    std::cout << " \t[OK]" << std::endl;
}


void sfera_webserver::http_server::run()
{
    _ioservice.run();
}


void sfera_webserver::http_server::_handleAccept(const b_sys::error_code &ec)
{
    if (ec) {
        std::cout << "accept error" << std::endl;
        return;
    }

    auto remoteEndpoint =_acceptedSocket->remote_endpoint();
    std::cout << "accepted connection from " << remoteEndpoint.address().to_string() << ":"
        << remoteEndpoint.port() << std::endl;

    auto connection = new client_connection(*this, _acceptedSocket);
    _startAsyncRequestReading(connection);

    _startAsyncAccept();
}


void sfera_webserver::http_server::_startAsyncAccept()
{
    _acceptedSocket = new b_ip::tcp::socket(_ioservice);
    _acceptor.async_accept(*_acceptedSocket, [this](const b_sys::error_code &ec) { _handleAccept(ec); });
}


sfera_webserver::http_server::~http_server()
{
    delete _acceptedSocket;
}


void sfera_webserver::http_server::_handleRequestReady(http_request *req, client_connection *connection)
{
    std::cout << "got request: " << req->strMethod() << " " << req->resourceURL() << std::endl;

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
                               std::cout << "connection terminated: timeout" << std::endl;
                               connection->close();
                               delete connection;
                           }
    );
}
