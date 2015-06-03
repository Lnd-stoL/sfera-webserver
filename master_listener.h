
#ifndef SFERA_WEBSERVER_HTTP_CONNECTION_LISTENER_H
#define SFERA_WEBSERVER_HTTP_CONNECTION_LISTENER_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include "worker.h"

#include <functional>
#include <map>
#include <thread>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{

    class master_listener
    {
    private:
        struct worker_handler
        {
            pid_t pid;
            int communicationChannel[2];
        };

    private:
        unsigned _maxWorkers = 4;

    private:
        b_asio::io_service _ioservice;
        b_ip::tcp::socket *_acceptedSocket = nullptr;
        b_ip::tcp::acceptor _acceptor;

        http_server_config _serverConfig;
        std::map<pid_t, worker_handler> _workers;
        b_asio::signal_set _sigChildSet;


    private:
        void _startAsyncAccept();
        void _handleAccept(const b_sys::error_code &ec);

        void _workerProcess(worker_handler workerHandler);
        bool _checkWorkers();
        void _handleWorkerTerminated();

    public:
        master_listener(const http_server_config &&config);
        ~master_listener();

        void run();
    };

}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HTTP_CONNECTION_LISTENER_H
