#ifndef CONFIG__H
#define CONFIG__H

#include <iostream>
#include <chrono>

class Config {
public:
    Config(const std::string &configPath);
    Config();
    ~Config() = default;
    Config(const Config &) = default;
    Config(Config &&) = default;
    Config &operator=(const Config &) = default;
    Config &operator=(Config &&) = default;
    
    std::uint16_t port;
    
private:
    static const std::string argPort;
    
    void loadFromFile(const std::string &path);
    void loadDefaults();
};

#endif