
#ifndef SFERA_WEBSERVER_HEAD_REQUEST_PROCESSOR_H
#define SFERA_WEBSERVER_HEAD_REQUEST_PROCESSOR_H

//----------------------------------------------------------------------------------------------------------------------

#include "request_processor.h"
#include "file_info.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class head_request_processor : public request_processor
    {
    private:
        file_info _requestedFileInfo;


    protected:
        virtual void _handleResponseSent();
        virtual http_response::status _determineStatus();
        virtual void _fillResponse();

    public:
        head_request_processor(http_request *req,  client_connection *connection, callback_t readyCallback);
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HEAD_REQUEST_PROCESSOR_H
