#include <fstream>
#include <cstdint>

#include <boost/program_options.hpp>

#include "Config.h"

const std::string Config::argServerPort = "serverPort";

const std::string Config::argDatabaseHost = "databaseHost";
const std::string Config::argDatabaseUser = "databaseUser";
const std::string Config::argDatabaseName = "databaseName";
const std::string Config::argDatabasePort = "databasePort";

const std::string Config::argUrlPrefixForMapImage = "urlPrefixForMapImage";

Config::Config(const std::string &configPath) {
    loadFromFile(configPath);
}

void Config::loadFromFile(const std::string &path) {
    namespace po = boost::program_options;

    po::options_description opts("Config file");
    opts.add_options()(argServerPort.c_str(), po::value(&serverPort)->default_value(9090));

    opts.add_options()(argDatabaseHost.c_str(),
                       po::value(&databaseHost)->default_value("localhost"));
    opts.add_options()(argDatabaseUser.c_str(), po::value(&databaseUser)->required());
    opts.add_options()(argDatabaseName.c_str(), po::value(&databaseName)->required());
    opts.add_options()(argDatabasePort.c_str(), po::value(&databasePort)->default_value(5432));

    opts.add_options()(argUrlPrefixForMapImage.c_str(),
                       po::value(&urlPrefixForMapImage)->required());

    po::variables_map vm;

    std::ifstream ifs(path.c_str());
    po::store(po::parse_config_file(ifs, opts), vm);
    po::notify(vm);
}