
#include "worker.h"
#include "libancillary/ancillary.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::worker::worker(int masterCommunicationSocket_) :
        _masterCommunicationSocket(masterCommunicationSocket_)
{
    _acceptorThread = std::thread(std::bind(&worker::_acceptorThreadRoutine, this));
}


void sfera_webserver::worker::run(http_server *server)
{
    _server = server;

    boost::asio::io_service::work work(_server->ioService());
    std::unique_lock<std::mutex> lock(_acceptingMutex);

    while (_working) {
        _acceptingCondition.wait(lock);
        _server->run();
    }
}


void sfera_webserver::worker::_acceptorThreadRoutine()
{
    while (_working) {
        int newClientFd = 0;
        ancil_recv_fd(_masterCommunicationSocket, &newClientFd);

        b_ip::tcp::socket* acceptedSocket = new b_ip::tcp::socket(_server->ioService());
        acceptedSocket->assign(b_ip::tcp::v4(), newClientFd);

        _server->ioService().post([this, acceptedSocket]() {
            _server->handleClientConnection(acceptedSocket);
        });

        _acceptingCondition.notify_one();
    }
}


sfera_webserver::worker::~worker()
{
    _working = false;
    _acceptorThread.join();
}
