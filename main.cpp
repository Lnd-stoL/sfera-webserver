
#include "http_server_config.h"
#include "master_listener.h"
#include "logger.hpp"

#include <boost/program_options.hpp>
namespace b_po = boost::program_options;

//----------------------------------------------------------------------------------------------------------------------

b_src::severity_logger<severity_level>  g_logger;

//----------------------------------------------------------------------------------------------------------------------

void fillProgramOptions(b_po::options_description& desc)
{
    desc.add_options()
            ("help",  "Show this help")
            ("dir,d",  b_po::value<std::string>(), "WWW home directory")
            ("host,h", b_po::value<std::string>(), "Server binding ip")
            ("port,p", b_po::value<unsigned>(),    "Server listening port");
}


void initLog()
{
    b_logging::add_file_log
    (
        b_keywords::file_name = "sfera-webserver-log_%N.log",
        b_keywords::rotation_size = 10 * 1024 * 1024,
        b_keywords::time_based_rotation = b_sinks::file::rotation_at_time_point(0, 0, 0),
        b_keywords::format = "[%TimeStamp%]: %Message%",
        b_keywords::auto_flush = true
    );

    b_logging::add_console_log(std::cout, b_keywords::format = "[%TimeStamp%]: %Message%");

    /*
    b_logging::core::get()->set_filter
    (
        b_logging::trivial::severity >= b_logging::trivial::info
    );
    */

    b_logging::add_common_attributes();
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    initLog();

    sfera_webserver::http_server_config serverConfig;
    serverConfig.port = 8000;
    serverConfig.wwwHomeDir  = "./www_home";
    serverConfig.host = "127.0.0.1";

    b_po::options_description desc("Server usage");
    fillProgramOptions(desc);

    b_po::variables_map cmdLineOptions;
    b_po::store(b_po::command_line_parser(argc, argv).options(desc).run(), cmdLineOptions);
    b_po::notify(cmdLineOptions);

    if (cmdLineOptions.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (cmdLineOptions.count("port")  == 1)  serverConfig.port        = cmdLineOptions["port"].as<unsigned>();
    if (cmdLineOptions.count("dir")   == 1)  serverConfig.wwwHomeDir  = cmdLineOptions["dir"].as<std::string>();
    if (cmdLineOptions.count("host")  == 1)  serverConfig.host        = cmdLineOptions["host"].as<std::string>();

    sfera_webserver::master_listener listener(std::move(serverConfig));
    listener.run();
    return 0;
}
