#include <iostream>

#include "io/input/HelloMsg.h"
#include "io/utils.h"
#include "CommandHandler.h"
#include "commands.h"

namespace server {

CommandHandler::CommandHandler(db::Database &db) : db(db), srv(nullptr) {
}

void CommandHandler::setServer(apache::thrift::server::TServer *srv) {
    this->srv = srv;
}

void CommandHandler::shutdown() {
    LOG(INFO) << __func__ << " start";

    assert(srv);
    srv->stop();

    LOG(INFO) << __func__ << " end";
}

#pragma mark - PING

std::int32_t CommandHandler::ping(const communication::HelloMsg &msg) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << msg;

    std::int32_t output = withExceptionTranslation([&]() {
        io::input::HelloMsg input(msg);
        return input.num;
    });

    LOG(INFO) << "output: " << output;
    LOG(INFO) << __func__ << " end";

    return output;
}

#pragma mark - MAP

void CommandHandler::getNewMapImages(communication::NewMapImagesResponse &response,
                                     const communication::NewMapImagesRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::NewMapImagesRequest input(request);
        io::output::NewMapImagesResponse output = command::GetNewMapImagesCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::setMapImage(const communication::SetMapImageRequest &request) {
    std::lock_guard<std::mutex> lock(setMapLock);

    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::SetMapImageRequest input(request);
        command::SetMapImageCommand{db}(input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getMapImageTiles(communication::MapImageTilesResponse &response,
                                      const communication::MapImageTilesRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::MapImageTilesRequest input(request);
        io::output::MapImageTilesResponse output = command::GetMapImageTilesCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXHIBITS

void CommandHandler::getNewExhibits(communication::NewExhibitsResponse &response,
                                    const communication::NewExhibitsRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::NewExhibitsRequest input(request);
        io::output::NewExhibitsResponse output = command::GetNewExhibitsCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::createExhibit(communication::Exhibit &response,
                                   const communication::CreateExhibitRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::CreateExhibitRequest input(request);
        io::Exhibit output = command::CreateExhibitCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllExhibits(std::vector<communication::Exhibit> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllExhibitsCommand{db}();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::setExhibitFrame(const communication::SetExhibitFrameRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::SetExhibitFrameRequest input(request);
        command::SetExhibitFrameCommand{db}(input);
    });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXPERIMENTS

void CommandHandler::getCurrentExperiment(communication::CurrentExperimentResponse &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        io::output::CurrentExperimentResponse output = command::GetCurrentExperimentCommand{db}();
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - REPORTS

std::int32_t CommandHandler::getIdForNewReport() {
    LOG(INFO) << __func__ << " start";

    std::int32_t response =
        withExceptionTranslation([&]() { return command::ReserveIdForReportCommand{db}(); });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";

    return response;
}

void CommandHandler::saveReport(const communication::RawReport &report) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << report;

    withExceptionTranslation([&]() {
        io::input::RawReport input(report);
        command::SaveRawReportCommand{db}(input);
    });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - ACTIONS

void CommandHandler::createAction(communication::Action &response,
                                  const communication::CreateActionRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::CreateActionRequest input(request);
        io::Action output = command::CreateActionCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllActions(std::vector<communication::Action> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllActionsCommand{db}();
        response = ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - SIMPLE QUESTIONS

void CommandHandler::createSimpleQuestion(
    communication::SimpleQuestion &response,
    const communication::CreateSimpleQuestionRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        io::input::CreateSimpleQuestionRequest input(request);
        io::SimpleQuestion output = command::CreateSimpleQuestionCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllSimpleQuestions(std::vector<communication::SimpleQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllSimpleQuestionsCommand{db}();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}
}