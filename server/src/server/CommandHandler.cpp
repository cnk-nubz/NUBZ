#include "CommandHandler.h"
#include "commands.h"
#include "io/input/HelloMsg.h"
#include "io/utils.h"

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
        auto input = io::input::HelloMsg{msg};
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
        auto input = io::input::NewMapImagesRequest{request};
        auto output = command::GetNewMapImagesCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::setMapImage(communication::MapImage &response,
                                 const communication::SetMapImageRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::SetMapImageRequest{request};
        auto output = command::SetMapImageCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXHIBITS

void CommandHandler::getNewExhibits(communication::NewExhibitsResponse &response,
                                    const communication::NewExhibitsRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::NewExhibitsRequest{request};
        auto output = command::GetNewExhibitsCommand{db}(input);
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
        auto input = io::input::CreateExhibitRequest{request};
        auto output = command::CreateExhibitCommand{db}(input);
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
        auto input = io::input::SetExhibitFrameRequest{request};
        command::SetExhibitFrameCommand{db}(input);
    });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXPERIMENTS

void CommandHandler::getCurrentExperiment(communication::CurrentExperimentResponse &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetCurrentExperimentCommand{db}();
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::createExperiment(const communication::CreateExperimentRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CreateExperimentRequest{request};
        command::CreateExperimentCommand{db}(input);
    });

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
        auto input = io::input::RawReport(report);
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
        auto input = io::input::CreateActionRequest(request);
        auto output = command::CreateActionCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllActions(std::vector<communication::Action> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllActionsCommand{db}();
        response = io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - QUESTIONS

void CommandHandler::getAllQuestions(communication::QuestionsList &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllQuestionsCommand{db}();
        response = output.toThrift();
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
        auto input = io::input::CreateSimpleQuestionRequest(request);
        auto output = command::CreateSimpleQuestionCommand{db}(input);
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

#pragma mark - MULTIPLE CHOICE QUESTIONS

void CommandHandler::createMultipleChoiceQuestion(
    communication::MultipleChoiceQuestion &response,
    const communication::CreateMultipleChoiceQuestionRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CreateMultipleChoiceQuestionRequest(request);
        auto output = command::CreateMultipleChoiceQuestionCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllMultipleChoiceQuestions(
    std::vector<communication::MultipleChoiceQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllMultipleChoiceQuestionsCommand{db}();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - SORT QUESTIONS

void CommandHandler::createSortQuestion(communication::SortQuestion &response,
                                        const communication::CreateSortQuestionRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CreateSortQuestionRequest(request);
        auto output = command::CreateSortQuestionCommand{db}(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllSortQuestions(std::vector<communication::SortQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::GetAllSortQuestionsCommand{db}();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}
}