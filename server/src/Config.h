#ifndef CONFIG__H
#define CONFIG__H

#include <iostream>
#include <chrono>

class Config {
public:
    Config(const std::string &configPath);
    ~Config() = default;
    Config(const Config &) = default;
    Config(Config &&) = default;
    Config &operator=(const Config &) = default;
    Config &operator=(Config &&) = default;

    std::uint16_t serverPort;

    std::string databaseHost;
    std::string databaseUser;
    std::string databaseName;
    std::uint16_t databasePort;

    std::string urlPrefixForMapImage;

private:
    static const std::string argServerPort;

    static const std::string argDatabaseHost;
    static const std::string argDatabaseUser;
    static const std::string argDatabaseName;
    static const std::string argDatabasePort;

    static const std::string argUrlPrefixForMapImage;

    void loadFromFile(const std::string &path);
};

#endif