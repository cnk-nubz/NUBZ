#include <fstream>
#include <cstdint>

#include <boost/program_options.hpp>

#include "Config.h"

const std::string Config::argPort = "port";

Config::Config() {
    loadDefaults();
}

Config::Config(const std::string &configPath) {
    loadDefaults();
    loadFromFile(configPath);
}

void Config::loadDefaults() {
    port = 9090;
}

void Config::loadFromFile(const std::string &path) {
    namespace po = boost::program_options;

    po::options_description opts("Config file");
    opts.add_options()(argPort.c_str(), po::value<std::uint16_t>(&port));

    po::variables_map vm;

    std::ifstream ifs(path.c_str());
    po::store(po::parse_config_file(ifs, opts), vm);
    po::notify(vm);
}