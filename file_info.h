
#ifndef SFERA_WEBSERVER_FILE_INFO_H
#define SFERA_WEBSERVER_FILE_INFO_H

//----------------------------------------------------------------------------------------------------------------------

#include "asio_headers.h"

#include <ctime>
#include <map>

//----------------------------------------------------------------------------------------------------------------------

namespace sfera_webserver
{
    class file_info
    {
    private:
        b_fs::path _path = "";
        std::map<std::string, std::string> _mimeTypesByExt;

    private:
        void _fillMimeTypesByExt();


    public:
        file_info()  { };
        file_info(b_fs::path path);

        size_t size() const;
        b_time::ptime lastModified() const;
        std::string extension() const;
        std::string mimeType() const;
        bool exists() const;
        bool isDirectory() const;
        b_fs::path path() const;
    };
}

//----------------------------------------------------------------------------------------------------------------------

#endif    //SFERA_WEBSERVER_FILE_INFO_H
