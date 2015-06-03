
#ifndef SFERA_WEBSERVER_HTTP_SERVER_CONFIG_H
#define SFERA_WEBSERVER_HTTP_SERVER_CONFIG_H

//----------------------------------------------------------------------------------------------------------------------

#include <string>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{

    struct http_server_config
    {
        std::string  wwwHomeDir;
        std::string  host;
        unsigned     port;
    };

}

//----------------------------------------------------------------------------------------------------------------------

#endif //SFERA_WEBSERVER_HTTP_SERVER_CONFIG_H
