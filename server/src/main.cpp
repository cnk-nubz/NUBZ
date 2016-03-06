#include <cstdlib>
#include <iostream>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <db/Database.h>

#include <utils/Config.h>
#include <utils/log.h>

#include <server/CommandHandler.h>
#include <server/utils/PathHelper.h>

INITIALIZE_EASYLOGGINGPP

boost::optional<utils::Config> parseArguments(int argc, char *argv[]);
void runServer(std::uint16_t port, db::Database &db);

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    if (auto cfg = parseArguments(argc, argv)) {
        auto config = cfg.value();

        server::utils::PathHelper::getInstance().setMapImgUrlPrefix(config.urlPrefixForMapImage);
        server::utils::PathHelper::getInstance().setTileUrlPrefix(config.urlPrefixForMapImage);
        server::utils::PathHelper::getInstance().setTmpDir(config.tmpFolderPath);
        server::utils::PathHelper::getInstance().setPublicDir(config.publicFolderPath);
        server::utils::PathHelper::getInstance().setMapTilesDir(config.mapTilesFolderPath);

        db::Database db(
            config.databaseUser, config.databaseName, config.databaseHost, config.databasePort);
        runServer(config.serverPort, db);
    }
}

boost::optional<utils::Config> parseArguments(int argc, char *argv[]) {
    static const char *configFilePathArg = "cfg";
    static const char *helpArg = "help";
    namespace po = boost::program_options;

    auto path = std::string{};
    auto opts = po::options_description{"Allowed options"};

    opts.add_options()(helpArg, "produce help message");
    opts.add_options()(configFilePathArg, po::value<std::string>(&path), "set path to config file");

    auto vm = po::variables_map{};
    try {
        po::store(po::parse_command_line(argc, argv, opts), vm);
        po::notify(vm);

        if (vm.count(helpArg)) {
            std::cout << opts;
            return {};
        }

        if (vm.count(configFilePathArg)) {
            return utils::Config(path);
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

    boost::shared_ptr<::server::CommandHandler> handler(new ::server::CommandHandler(db));
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
