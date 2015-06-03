
#include "master_listener.h"
#include "logger.hpp"

#include "libancillary/ancillary.h"
#include <sys/wait.h>

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::master_listener::master_listener(const http_server_config &&config) :
        _acceptor(_ioservice,
                  b_ip::tcp::endpoint(b_ip::address::from_string(config.host), (unsigned short)config.port), true),
        _serverConfig(config),
        _sigChildSet(_ioservice, SIGCHLD)
{
    BOOST_LOG(g_logger) << "starting sfera-webserver at " << _serverConfig.host << ":" << _serverConfig.port;
    _acceptor.listen();
}


void sfera_webserver::master_listener::_handleAccept(const b_sys::error_code &ec)
{
    if (ec) {
        BOOST_LOG(g_logger) << "accept error";
        _startAsyncAccept();
        return;
    }

    if (_checkWorkers()) {
        std::terminate();
    };

    auto remoteEndpoint =_acceptedSocket->remote_endpoint();
    BOOST_LOG(g_logger) << "accepted connection from " << remoteEndpoint.address().to_string() << ":"
        << remoteEndpoint.port();

    int newConnectionFd = _acceptedSocket->native_handle();

    auto worker = _workers.begin();
    std::advance(worker, rand() % _workers.size());
    ancil_send_fd(worker->second.communicationChannel[1], newConnectionFd);
    _acceptedSocket->close();

    _startAsyncAccept();
}


void sfera_webserver::master_listener::_startAsyncAccept()
{
    _acceptedSocket = new b_ip::tcp::socket(_ioservice);
    _acceptor.async_accept(*_acceptedSocket, [this](const b_sys::error_code &ec) { _handleAccept(ec); });
}


sfera_webserver::master_listener::~master_listener()
{
    delete _acceptedSocket;
}


void sfera_webserver::master_listener::run()
{
    if (_checkWorkers()) return;

    _startAsyncAccept();
    _sigChildSet.async_wait(boost::bind(&master_listener::_handleWorkerTerminated, this));

    _ioservice.run();
}


void sfera_webserver::master_listener::_workerProcess(worker_handler newWorkerHandler)
{
    ::close(newWorkerHandler.communicationChannel[1]);

    _ioservice.notify_fork(b_asio::io_service::fork_child);

    worker newWorker(newWorkerHandler.communicationChannel[0]);
    auto server = new http_server(std::move(_serverConfig), &_ioservice);
    newWorker.run(server);

    delete server;
    ::close(newWorkerHandler.communicationChannel[0]);
    return;
}


// returns false is we are still in parent, true otherwise
bool sfera_webserver::master_listener::_checkWorkers()
{
    while (_workers.size() < _maxWorkers) {
        worker_handler newWorkerHandler = {};
        ::socketpair(PF_UNIX, SOCK_DGRAM, 0, newWorkerHandler.communicationChannel);
        _ioservice.notify_fork(b_asio::io_service::fork_prepare);

        pid_t childPid = fork();
        if (childPid == 0) {
            _workerProcess(newWorkerHandler);
            return true;
        }

        _ioservice.notify_fork(b_asio::io_service::fork_parent);

        BOOST_LOG(g_logger) << "spawned worker process #" << _workers.size() << "  pid " << childPid;
        ::close(newWorkerHandler.communicationChannel[0]);
        newWorkerHandler.pid = childPid;
        _workers[childPid] = newWorkerHandler;
    }

    return false;
}


void sfera_webserver::master_listener::_handleWorkerTerminated()
{
    int status = 0;
    pid_t terminatedPid = ::wait(&status);
    _workers.erase(terminatedPid);

    BOOST_LOG(g_logger) << "worker terminated  pid " << terminatedPid;

    _sigChildSet.async_wait(boost::bind(&master_listener::_handleWorkerTerminated, this));
}
