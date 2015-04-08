
#include "http_standard_replies.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::http_standard_replies::http_standard_replies()
{
    _shortDescriptions[http_response::notFound] = "NotFound";
    _shortDescriptions[http_response::ok]       = "OK";
}


const std::string& sfera_webserver::http_standard_replies::shortDescriptionFor(http_response::status status) const
{
    return _shortDescriptions[status];
}


std::string sfera_webserver::http_standard_replies::pageContentFor404(std::string requestedFile) const
{
    std::ostringstream html;
    html << "<html>";
    html << "<head><title>404 - Not Found</title></head>";
    html << "<body> <h1><b>404 - Not Found</b></h1> <br><hr> <h3>" << requestedFile;
    html << "</h3></body></html>";

    return html.str();
}
