#include <iostream>

#include "external/easylogging++.h"

#include "CommandHandler.h"
#include "command/commands.h"
#include "io/io.h"

CommandHandler::CommandHandler(db::Database &db) : db(db), srv(nullptr) {
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

void CommandHandler::getMapImages(communication::MapImagesResponse &response,
                                  const communication::MapImagesRequest &request) {
    LOG(INFO) << "getMapImages start";
    LOG(INFO) << "input: " << request;

    try {
        io::input::MapImagesRequest input(request);

        command::GetMapImagesCommand cmd(db);
        io::output::MapImagesResponse output = cmd.perform(input);

        response = output.toThrift();
    } catch (std::exception &e) {
        LOG(ERROR) << e.what();
        throw communication::InternalError{};
    }

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getMapImages end";
}

void CommandHandler::setMapImage(const communication::SetMapImageRequest &request) {
    LOG(INFO) << "setMapImage start";
    LOG(INFO) << "input: " << request;

    try {
        io::input::SetMapImageRequest input(request);
        
        command::SetMapImageCommand cmd(db);
        cmd.perform(input);
    } catch (command::InvalidInput &e) {
        LOG(ERROR) << e.what();
        throw communication::InvalidData{};
    } catch (std::exception &e) {
        LOG(ERROR) << e.what();
        throw communication::InternalError{};
    }

    LOG(INFO) << "setMapImage end";
}