#ifndef SERVER_COMMAND_HANDLER__H
#define SERVER_COMMAND_HANDLER__H

#include <thrift/server/TServer.h>

#include <utils/log.h>

#include <db/Database.h>

#include <communication/Server.h>

#include "io/InvalidInput.h"

namespace server {

class CommandHandler : public communication::ServerIf {
public:
    CommandHandler(db::Database &db);

    void setServer(apache::thrift::server::TServer *srv);

    virtual void shutdown() override;

    virtual int32_t ping(const communication::HelloMsg &msg) override;

    virtual void getNewMapImages(communication::NewMapImagesResponse &response,
                                 const communication::NewMapImagesRequest &request) override;
    virtual void getMapImageTiles(communication::MapImageTilesResponse &response,
                                  const communication::MapImageTilesRequest &request) override;
    virtual void setMapImage(const communication::SetMapImageRequest &request) override;

    virtual void getNewExhibits(communication::NewExhibitsResponse &response,
                                const communication::NewExhibitsRequest &request) override;
    virtual void createExhibit(communication::Exhibit &response,
                               const communication::CreateExhibitRequest &request) override;
    virtual void getAllExhibits(std::vector<communication::Exhibit> &response) override;
    virtual void setExhibitFrame(const communication::SetExhibitFrameRequest &request) override;

    virtual void getCurrentExperiment(communication::CurrentExperimentResponse &response) override;
    virtual int32_t getIdForNewReport() override;

    virtual void saveReport(const communication::RawReport &report) override;

    virtual void createAction(communication::Action &response,
                              const communication::CreateActionRequest &request) override;
    virtual void getAllActions(std::vector<communication::Action> &response) override;

    virtual void getAllQuestions(communication::QuestionsList &response) override;

    virtual void createSimpleQuestion(
        communication::SimpleQuestion &response,
        const communication::CreateSimpleQuestionRequest &request) override;
    virtual void getAllSimpleQuestions(
        std::vector<communication::SimpleQuestion> &response) override;

    virtual void createMultipleChoiceQuestion(
        communication::MultipleChoiceQuestion &response,
        const communication::CreateMultipleChoiceQuestionRequest &request) override;
    virtual void getAllMultipleChoiceQuestions(
        std::vector<communication::MultipleChoiceQuestion> &response) override;

private:
    db::Database &db;
    apache::thrift::server::TServer *srv;

    std::mutex setMapLock;

    template <class F>
    std::result_of_t<F()> withExceptionTranslation(F &&f);
};

template <class F>
std::result_of_t<F()> CommandHandler::withExceptionTranslation(F &&f) {
    try {
        return f();
    } catch (server::io::InvalidInput &e) {
        LOG(INFO) << "InvalidInput: " << e.what();
        throw e.toThrift();
    } catch (std::exception &e) {
        LOG(INFO) << "InternalError: " << e.what();
        throw communication::InternalError{};
    } catch (...) {
        LOG(INFO) << "InternalError";
        throw communication::InternalError();
    }
}
}

#endif