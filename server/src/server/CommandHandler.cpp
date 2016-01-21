#include <iostream>

#include "CommandHandler.h"
#include "commands.h"
#include "io.h"

namespace server {

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

    std::int32_t output = withExceptionTranslation([&]() {
        io::input::HelloMsg input(msg);
        return input.num;
    });

    LOG(INFO) << "output: " << output;
    LOG(INFO) << "ping end";

    return output;
}

void CommandHandler::getMapImages(communication::MapImagesResponse &response,
                                  const communication::MapImagesRequest &request) {
    LOG(INFO) << "getMapImages start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::MapImagesRequest input(request);
        io::output::MapImagesResponse output = command::GetMapImagesCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getMapImages end";
}

void CommandHandler::setMapImage(const communication::SetMapImageRequest &request) {
    std::lock_guard<std::mutex> lock(setMapLock);

    LOG(INFO) << "setMapImage start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::SetMapImageRequest input(request);
        command::SetMapImageCommand{db}(input);
    });

    LOG(INFO) << "setMapImage end";
}

void CommandHandler::getMapImageTiles(communication::MapImageTilesResponse &response,
                                      const communication::MapImageTilesRequest &request) {
    LOG(INFO) << "getMapImageTiles start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::MapImageTilesRequest input(request);
        io::output::MapImageTilesResponse output = command::GetMapImageTilesCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getMapImageTiles end";
}

void CommandHandler::getExhibits(communication::ExhibitsResponse &response,
                                 const communication::ExhibitsRequest &request) {
    LOG(INFO) << "getExhibits start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::ExhibitsRequest input(request);
        io::output::ExhibitsResponse output = command::GetExhibitsCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getExhibits end";
}

std::int32_t CommandHandler::getIdForNewReport() {
    LOG(INFO) << "getIdForNewReport start";

    std::int32_t response =
        withExceptionTranslation([&]() { return command::ReserveIdForReportCommand{db}(); });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getIdForNewReport end";

    return response;
}

void CommandHandler::saveReport(const communication::RawReport &report) {
    LOG(INFO) << "saveReport start";
    LOG(INFO) << "input: " << report;

    withExceptionTranslation([&]() {
        io::input::RawReport input(report);
        command::SaveRawReportCommand{db}(input);
    });

    LOG(INFO) << "saveReport end";
}

void CommandHandler::createNewExhibit(communication::NewExhibitResponse &response,
                                      const communication::NewExhibitRequest &request) {
    LOG(INFO) << "createNewExhibit start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::NewExhibitRequest input(request);
        io::output::NewExhibitResponse output = command::CreateNewExhibitCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "createNewExhibit end";
}

void CommandHandler::setExhibitFrame(const communication::SetExhibitFrameRequest &request) {
    LOG(INFO) << "setExhibitsFrames start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::SetExhibitFrameRequest input(request);
        command::SetExhibitFrameCommand{db}(input);
    });

    LOG(INFO) << "setExhibitsFrames end";
}

void CommandHandler::getCurrentExperiment(communication::CurrentExperimentResponse &response) {
    LOG(INFO) << "getExperimentData start";

    withExceptionTranslation([&]() {
        io::output::CurrentExperimentResponse output = command::GetCurrentExperimentCommand{db}();
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getExperimentData end";
}

void CommandHandler::createNewAction(communication::NewActionResponse &response,
                                     const communication::NewActionRequest &request) {
    LOG(INFO) << "createNewAction start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() { LOG(INFO) << "not implemented"; });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "createNewAction end";
}

void CommandHandler::getActions(communication::ActionsResponse &response) {
    LOG(INFO) << "getActions start";

    withExceptionTranslation([&]() { LOG(INFO) << "not implemented"; });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << "getActions end";
}
}