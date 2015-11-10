#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <boost/program_options.hpp>

#include "external/easylogging++.h"

#include "CommandHandler.h"
#include "Config.h"

INITIALIZE_EASYLOGGINGPP

Config *parseArguments(int argc, char *argv[]);
void runServer(std::uint16_t port);

int main(int argc, char *argv[]) {
    Config *cfg = parseArguments(argc, argv);
    if (!cfg) {
        return 0;
    }

    runServer(cfg->port);
}

Config *parseArguments(int argc, char *argv[]) {
    static const char *configFilePathArg = "cfg";
    static const char *helpArg = "help";
    namespace po = boost::program_options;

    std::string path;
    po::options_description opts("Allowed options");

    opts.add_options()(helpArg, "produce help message");
    opts.add_options()(configFilePathArg, po::value<std::string>(&path), "set path to config file");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, opts), vm);
    } catch (const boost::program_options::error &e) {
        std::cerr << e.what() << std::endl;
        std::cout << opts;
        return nullptr;
    }
    po::notify(vm);
    
    if (vm.count(helpArg)) {
        std::cout << opts;
        return nullptr;
    }

    if (vm.count(configFilePathArg)) {
        return new Config(path);
    }

    return new Config();
}

void runServer(std::uint16_t port) {
    using namespace apache::thrift;
    using namespace apache::thrift::protocol;
    using namespace apache::thrift::transport;
    using namespace apache::thrift::server;

    boost::shared_ptr<CommandHandler> handler(new CommandHandler());
    boost::shared_ptr<TProcessor> processor(new communication::ServerProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    handler->setServer(&server);
    
    LOG(INFO) << "Starting server...";
    LOG(INFO) << "Port: " << port;
    
    server.serve();
}