
#ifndef SFERA_WEBSERVER_ASIO_HEADERS_H
#define SFERA_WEBSERVER_ASIO_HEADERS_H

//----------------------------------------------------------------------------------------------------------------------

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <string>

namespace b_logging  = boost::log;
namespace b_src      = boost::log::sources;
namespace b_sinks    = boost::log::sinks;
namespace b_keywords = boost::log::keywords;

namespace b_time = boost::posix_time;
namespace b_fs   = boost::filesystem;
namespace b_sys  = boost::system;
namespace b_asio = boost::asio;
namespace b_ip   = boost::asio::ip;

using namespace b_logging::trivial;

//----------------------------------------------------------------------------------------------------------------------

#endif   //SFERA_WEBSERVER_ASIO_HEADERS_H
