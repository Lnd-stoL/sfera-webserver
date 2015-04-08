
#ifndef SFERA_WEBSERVER_REQUEST_PROCESSOR_H
#define SFERA_WEBSERVER_REQUEST_PROCESSOR_H

//----------------------------------------------------------------------------------------------------------------------

#include "http_request.h"
#include "http_response.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class request_processor
    {
    public:
        typedef std::function<void(request_processor *)> callback_t;

    protected:
        client_connection *_connection = nullptr;
        http_request  *_request  = nullptr;
        http_response *_response = nullptr;

        callback_t _callback;


    protected:
        void _createAndSendResponse();

        virtual void _handleResponseSent() = 0;
        virtual http_response::status _determineStatus() = 0;
        virtual void _fillResponse() = 0;

    public:
        request_processor(http_request *req, client_connection *connection, callback_t readyCallback);
        virtual ~request_processor();

        void finalize();
        inline client_connection *connection()  { return _connection; }

    public:
        template<typename processor_t>
        static processor_t *processAsync(http_request *req, client_connection *connection, callback_t readyCallback);
    };
}

//----------------------------------------------------------------------------------------------------------------------

template<typename processor_t>
processor_t *sfera_webserver::request_processor::processAsync(sfera_webserver::http_request *req,
                                                              client_connection *connection,
                                                              request_processor::callback_t callback)
{
    return new processor_t(req, connection, callback);
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_REQUEST_PROCESSOR_H
