#include <cstdint>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "Config.h"

namespace utils {

const char *Config::argServerPort = "serverPort";

const char *Config::argDatabaseHost = "databaseHost";
const char *Config::argDatabaseUser = "databaseUser";
const char *Config::argDatabaseName = "databaseName";
const char *Config::argDatabasePort = "databasePort";

const char *Config::argUrlPrefixForMapImage = "urlPrefixForMapImage";
const char *Config::argUrlPrefixForMapImageTiles = "urlPrefixForMapImageTiles";

const char *Config::argPublicFolderPath = "publicFolderPath";
const char *Config::argTmpFolderPath = "tmpFolderPath";
const char *Config::argMapTilesFolderPath = "mapTilesFolderPath";
const char *Config::argExcelReportsFolderPath = "excelReportsFolderPath";

const char *Config::argLogOutPath = "logOutputPath";

Config::Config(const std::string &configPath) {
    loadFromFile(configPath);
}

void Config::loadFromFile(const std::string &path) {
    namespace po = boost::program_options;

    po::options_description opts("Config file");
    opts.add_options()(argServerPort, po::value(&serverPort)->default_value(9090));

    opts.add_options()(argDatabaseHost, po::value(&databaseHost)->default_value("localhost"));
    opts.add_options()(argDatabaseUser, po::value(&databaseUser)->required());
    opts.add_options()(argDatabaseName, po::value(&databaseName)->required());
    opts.add_options()(argDatabasePort, po::value(&databasePort)->default_value(5432));

    opts.add_options()(argUrlPrefixForMapImage, po::value(&urlPrefixForMapImage)->required());
    opts.add_options()(argUrlPrefixForMapImageTiles,
                       po::value(&urlPrefixForMapImageTiles)->required());

    opts.add_options()(argPublicFolderPath, po::value(&publicFolderPath)->required());
    opts.add_options()(argTmpFolderPath, po::value(&tmpFolderPath)->required());
    opts.add_options()(argMapTilesFolderPath, po::value(&mapTilesFolderPath)->required());
    opts.add_options()(argExcelReportsFolderPath, po::value(&excelReportsFolderPath)->required());

    opts.add_options()(argLogOutPath, po::value(&logOutPath)->required());

    po::variables_map vm;

    std::ifstream ifs(path.c_str());
    po::store(po::parse_config_file(ifs, opts), vm);
    po::notify(vm);

    validate();
}

void Config::validate() const {
    validateDirectory(publicFolderPath);
    validateDirectory(tmpFolderPath);
    validateDirectory(mapTilesFolderPath);
    validateDirectory(excelReportsFolderPath);
}

void Config::validateDirectory(const std::string &path) const {
    namespace fs = boost::filesystem;
    if (!fs::is_directory(fs::path(path))) {
        throw std::runtime_error(path + " is not a directory.");
    }
}
}