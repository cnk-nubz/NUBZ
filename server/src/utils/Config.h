#ifndef CONFIG__H
#define CONFIG__H

#include <chrono>
#include <iostream>

namespace utils {

class Config {
public:
    Config(const std::string &configPath);

    std::uint16_t serverPort;

    std::string databaseHost;
    std::string databaseUser;
    std::string databaseName;
    std::uint16_t databasePort;

    std::string urlPrefixForMapImage;
    std::string urlPrefixForMapImageTiles;

    std::string publicFolderPath;
    std::string tmpFolderPath;
    std::string mapTilesFolderPath;
    std::string excelReportsFolderPath;

    std::string logOutPath;

private:
    static const char *argServerPort;

    static const char *argDatabaseHost;
    static const char *argDatabaseUser;
    static const char *argDatabaseName;
    static const char *argDatabasePort;

    static const char *argUrlPrefixForMapImage;
    static const char *argUrlPrefixForMapImageTiles;

    static const char *argPublicFolderPath;
    static const char *argTmpFolderPath;
    static const char *argMapTilesFolderPath;
    static const char *argExcelReportsFolderPath;

    static const char *argLogOutPath;

    void loadFromFile(const std::string &path);
    void validate() const;
    void validateDirectory(const std::string &path) const;
};
}

#endif