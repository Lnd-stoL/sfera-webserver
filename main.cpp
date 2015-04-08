
#include "http_server.h"

#include <boost/program_options.hpp>
namespace b_po = boost::program_options;

//----------------------------------------------------------------------------------------------------------------------

void fillProgramOptions(b_po::options_description& desc)
{
    desc.add_options()
            ("help",  "Show this help")
            ("dir,d",  b_po::value<std::string>(), "WWW home directory")
            ("host,h", b_po::value<std::string>(), "Server binding ip")
            ("port,p", b_po::value<unsigned>(),    "Server listening port");
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    unsigned port = 8080;
    std::string homeDir  = "./www_home";
    std::string serverIP = "127.0.0.1";

    b_po::options_description desc("Server usage");
    fillProgramOptions(desc);

    b_po::variables_map cmdLineOptions;
    b_po::store(b_po::command_line_parser(argc, argv).options(desc).run(), cmdLineOptions);
    b_po::notify(cmdLineOptions);

    if (cmdLineOptions.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (cmdLineOptions.count("port") == 1)   port     = cmdLineOptions["port"].as<unsigned>();
    if (cmdLineOptions.count("dir")  == 1)   homeDir  = cmdLineOptions["dir"].as<std::string>();
    if (cmdLineOptions.count("host")  == 1)  serverIP = cmdLineOptions["dir"].as<std::string>();

    std::cout << "starting sfera-webserver at " << serverIP << ":" << port;
    sfera_webserver::http_server server(homeDir, serverIP, port);
    server.run();

    return 0;
}
