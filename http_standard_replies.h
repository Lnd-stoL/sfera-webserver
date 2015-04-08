
#ifndef SFERA_WEBSERVER_HTTP_STANDARD_REPLIES_H
#define SFERA_WEBSERVER_HTTP_STANDARD_REPLIES_H

//----------------------------------------------------------------------------------------------------------------------

#include <string>
#include "http_response.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class http_standard_replies
    {
    private:
        std::string _shortDescriptions[600];


    public:
        http_standard_replies();

        const std::string& shortDescriptionFor(http_response::status status) const;
        std::string pageContentFor404(std::string requestedFile) const;
    };
}


//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_HTTP_STANDARD_REPLIES_H
