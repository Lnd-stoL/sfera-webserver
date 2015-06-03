
#ifndef SFERA_WEBSERVER_WORKER_H
#define SFERA_WEBSERVER_WORKER_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"
#include "http_server.h"

#include <thread>
#include <mutex>
#include <condition_variable>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class worker
    {
    private:
        int _masterCommunicationSocket = -1;
        bool _working = true;
        http_server *_server = nullptr;

        std::thread _acceptorThread;
        std::mutex _acceptingMutex;
        std::condition_variable _acceptingCondition;


    private:
        void _acceptorThreadRoutine();


    public:
        worker(int masterCommunicationSocket_);
        ~worker();

        void run(http_server *server);

        inline int masterCommunicationSocket() const  { return _masterCommunicationSocket; }
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_WORKER_H
