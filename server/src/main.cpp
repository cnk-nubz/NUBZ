#include <iostream>
#include <cstdlib>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/concurrency/ThreadManager.h>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>

#include "external/easylogging++.h"

#include "CommandHandler.h"
#include "Config.h"
#include "FileHelper.h"
#include "db/Database.h"
#include "command/GetMapImagesCommand.h"
#include "command/GetMapImageTilesCommand.h"

INITIALIZE_EASYLOGGINGPP

boost::optional<Config> parseArguments(int argc, char *argv[]);
void runServer(std::uint16_t port, db::Database &db);

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    boost::optional<Config> cfg = parseArguments(argc, argv);
    if (!cfg) {
        return 0;
    }

    FileHelper::configure(cfg->tmpFolderPath, cfg->publicFolderPath, cfg->mapTilesFolderPath);
    command::GetMapImagesCommand::setUrlPathPrefix(cfg->urlPrefixForMapImage);
    command::GetMapImageTilesCommand::setUrlPathPrefix(cfg->urlPrefixForMapImageTiles);

    db::Database db(cfg->databaseUser, cfg->databaseName, cfg->databaseHost, cfg->databasePort);
    runServer(cfg->serverPort, db);
}

boost::optional<Config> parseArguments(int argc, char *argv[]) {
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
        po::notify(vm);

        if (vm.count(helpArg)) {
            std::cout << opts;
            return {};
        }

        if (vm.count(configFilePathArg)) {
            return Config(path);
        } else {
            std::cout << opts;
            return {};
        }
    } catch (const boost::program_options::error &e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}

void runServer(std::uint16_t port, db::Database &db) {
    using namespace apache::thrift;
    using namespace apache::thrift::protocol;
    using namespace apache::thrift::transport;
    using namespace apache::thrift::server;
    using namespace apache::thrift::concurrency;

    boost::shared_ptr<CommandHandler> handler(new CommandHandler(db));
    boost::shared_ptr<TProcessor> processor(new communication::ServerProcessor(handler));
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(4);
    boost::shared_ptr<PosixThreadFactory> threadFactory =
        boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    TNonblockingServer server(processor, protocolFactory, port, threadManager);
    handler->setServer(&server);

    LOG(INFO) << "Starting server...";
    LOG(INFO) << "Port: " << port;

    server.serve();
}
