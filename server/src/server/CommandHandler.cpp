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

void CommandHandler::getMapImages(
    std::map<communication::FloorNum, communication::MapImage> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::MapCommands{db}.getAll();
        for (auto &entry : output) {
            response[entry.first] = entry.second.toThrift();
        }
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
        auto output = command::MapCommands{db}.set(input);
        response = output.toThrift();
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::removeFloor(const std::int32_t floor) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << floor;

    withExceptionTranslation([&]() { command::MapCommands{db}.remove(floor); });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXHIBITS

void CommandHandler::getNewExhibits(communication::NewExhibitsResponse &response,
                                    const communication::NewExhibitsRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::NewExhibitsRequest{request};
        auto output = command::ExhibitCommands{db}.getNew(input);
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
        auto output = command::ExhibitCommands{db}.create(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllExhibits(std::vector<communication::Exhibit> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ExhibitCommands{db}.getAll();
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
        command::ExhibitCommands{db}.setFrame(input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::updateExhibit(communication::Exhibit &response,
                                   const communication::UpdateExhibitRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::UpdateExhibitRequest{request};
        auto output = command::ExhibitCommands{db}.update(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - EXPERIMENTS

void CommandHandler::getCurrentExperiment(communication::CurrentExperimentResponse &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ExperimentCommands{db}.getCurrent();
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
        command::ExperimentCommands{db}.create(input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::updateExperiment(const int32_t experimentId,
                                      const communication::CreateExperimentRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << experimentId << " " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CreateExperimentRequest{request};
        command::ExperimentCommands{db}.update(experimentId, input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::cloneExperiment(const communication::CloneRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CloneRequest{request};
        command::ExperimentCommands{db}.clone(input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getReadyExperiments(std::vector<communication::ExperimentInfo> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ExperimentCommands{db}.getAllReady();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getFinishedExperiments(std::vector<communication::ExperimentInfo> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ExperimentCommands{db}.getAllFinished();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getActiveExperiment(communication::SingleExperimentInfo &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ExperimentCommands{db}.getActive();
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getExperiment(communication::Experiment &response,
                                   const int32_t experimentId) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << experimentId;

    withExceptionTranslation([&]() {
        auto output = command::ExperimentCommands{db}.get(experimentId);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::startExperiment(const int32_t experimentId) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << experimentId;

    withExceptionTranslation([&]() { command::ExperimentCommands{db}.start(experimentId); });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::finishExperiment() {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() { command::ExperimentCommands{db}.finish(); });

    LOG(INFO) << __func__ << " end";
}

#pragma mark - REPORTS

std::int32_t CommandHandler::getIdForNewReport() {
    LOG(INFO) << __func__ << " start";

    std::int32_t response =
        withExceptionTranslation([&]() { return command::ReportCommands{db}.reserveID(); });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";

    return response;
}

void CommandHandler::saveReport(const communication::RawReport &report) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << report;

    withExceptionTranslation([&]() {
        auto input = io::input::RawReport(report);
        command::ReportCommands{db}.save(input);
    });

    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllReportsForExperiment(std::vector<communication::ReportInfo> &response,
                                                const int32_t experimentId) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << experimentId;

    withExceptionTranslation([&]() {
        auto output = command::ReportCommands{db}.getAllReportsForExperiment(experimentId);
        response = io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getExcelReport(communication::Filename &response, const int32_t reportId) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << reportId;

    withExceptionTranslation([&]() {
        auto output = command::ReportCommands{db}.getExcelReport(reportId);
        response = output;
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getCombinedExcelReport(communication::Filename &response,
                                            const int32_t experimentId) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << experimentId;

    withExceptionTranslation([&]() {
        auto output = command::ReportCommands{db}.getCombinedExcelReport(experimentId);
        response = output;
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - ACTIONS

void CommandHandler::createAction(communication::Action &response,
                                  const communication::CreateActionRequest &request) {
    LOG(INFO) << __func__ << " start";
    LOG(INFO) << "input: " << request;

    withExceptionTranslation([&]() {
        auto input = io::input::CreateActionRequest(request);
        auto output = command::ActionCommands{db}.create(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllActions(std::vector<communication::Action> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::ActionCommands{db}.getAll();
        response = io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

#pragma mark - QUESTIONS

void CommandHandler::getAllQuestions(communication::QuestionsList &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::QuestionCommands{db}.getAll();
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
        auto output = command::QuestionCommands{db}.createSimple(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllSimpleQuestions(std::vector<communication::SimpleQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::QuestionCommands{db}.getAllSimple();
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
        auto output = command::QuestionCommands{db}.createMultipleChoice(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllMultipleChoiceQuestions(
    std::vector<communication::MultipleChoiceQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::QuestionCommands{db}.getAllMultipleChoice();
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
        auto output = command::QuestionCommands{db}.createSort(input);
        response = output.toThrift();
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}

void CommandHandler::getAllSortQuestions(std::vector<communication::SortQuestion> &response) {
    LOG(INFO) << __func__ << " start";

    withExceptionTranslation([&]() {
        auto output = command::QuestionCommands{db}.getAllSort();
        response = server::io::ioToThrift(output);
    });

    LOG(INFO) << "output: " << response;
    LOG(INFO) << __func__ << " end";
}
}