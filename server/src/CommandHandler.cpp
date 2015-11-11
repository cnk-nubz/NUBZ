#include <iostream>

#include "external/easylogging++.h"

#include "CommandHandler.h"
#include "command/PingCommand.h"
#include "io/input/HelloMsg.h"

CommandHandler::CommandHandler() : srv(nullptr) {
}

void CommandHandler::setServer(apache::thrift::server::TServer *srv) {
    this->srv = srv;
}

void CommandHandler::shutdown() {
    LOG(INFO) << "shutdown start";
    
    assert(srv);
    srv->stop();
    
    LOG(INFO) << "shutdown end";
}

std::int32_t CommandHandler::ping(const communication::HelloMsg &msg) {
    LOG(INFO) << "ping start";
    LOG(INFO) << "input: " << msg;

    io::input::HelloMsg input(msg);

    command::PingCommand cmd;

    std::int32_t output = cmd.perform(msg);

    LOG(INFO) << "output: " << output;
    LOG(INFO) << "ping end";

    return output;
}

void CommandHandler::getMapImages(communication::MapImagesResponse &response, const communication::MapImagesRequest &request) {
    LOG(INFO) << "getMapImages start";
    LOG(INFO) << "input: " << request;
    
    LOG(INFO) << "not implemented";
    
    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getMapImages end";
}

void CommandHandler::setMapImage(const communication::SetMapImageRequest &request) {
    LOG(INFO) << "setMapImage start";
    LOG(INFO) << "input: " << request;
    
    LOG(INFO) << "not implemented";
    
    LOG(INFO) << "setMapImage end";
}