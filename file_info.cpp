
#include "file_info.h"

//----------------------------------------------------------------------------------------------------------------------

sfera_webserver::file_info::file_info(b_fs::path path) : _path(path)
{
    _fillMimeTypesByExt();
}


size_t sfera_webserver::file_info::size() const
{
    return b_fs::file_size(_path);
}


b_time::ptime sfera_webserver::file_info::lastModified() const
{
    return b_time::from_time_t(b_fs::last_write_time(_path));
}


std::string sfera_webserver::file_info::extension() const
{
    return _path.extension().string();
}


void sfera_webserver::file_info::_fillMimeTypesByExt()
{
    _mimeTypesByExt[""]     = "";
    _mimeTypesByExt["html"] = "text/html";
    _mimeTypesByExt["htm"]  = "text/html";
    _mimeTypesByExt["jpg"]  = "image/jpeg";
    _mimeTypesByExt["jpeg"] = "image/jpeg";
    _mimeTypesByExt["png"]  = "image/png";
    _mimeTypesByExt["gif"]  = "image/gif";
    _mimeTypesByExt["txt"]  = "text/plain";
    _mimeTypesByExt["xml"]  = "text/xml";
}


std::string sfera_webserver::file_info::mimeType() const
{
    if (extension().empty()) return "";

    auto it = _mimeTypesByExt.find(extension().substr(1));
    if (it == _mimeTypesByExt.end())  return "";

    return it->second;
}


bool sfera_webserver::file_info::exists() const
{
    return b_fs::exists(_path);
}


bool sfera_webserver::file_info::isDirectory() const
{
    return b_fs::is_directory(_path);
}


b_fs::path sfera_webserver::file_info::path() const
{
    return _path;
}
