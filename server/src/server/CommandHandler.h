#ifndef SERVER_COMMAND_HANDLER__H
#define SERVER_COMMAND_HANDLER__H

#include <thrift/server/TServer.h>

#include <external/easylogging++.h>

#include <db/Database.h>

#include <communication/Server.h>

#include "io/InvalidInput.h"

namespace server {

class CommandHandler : public communication::ServerIf {
public:
    CommandHandler(db::Database &db);

    void setServer(apache::thrift::server::TServer *srv);

    virtual void shutdown() override;
    virtual std::int32_t ping(const communication::HelloMsg &msg) override;

    virtual void getMapImages(communication::MapImagesResponse &response,
                              const communication::MapImagesRequest &request) override;
    virtual void setMapImage(const communication::SetMapImageRequest &request) override;

    virtual void getMapImageTiles(communication::MapImageTilesResponse &response,
                                  const communication::MapImageTilesRequest &request) override;

    virtual void getExhibits(communication::ExhibitsResponse &response,
                             const communication::ExhibitsRequest &request) override;

    virtual std::int32_t getIdForNewReport() override;
    virtual void saveReport(const communication::RawReport &report) override;

    virtual void createNewExhibit(communication::NewExhibitResponse &response,
                                  const communication::NewExhibitRequest &request) override;
    virtual void setExhibitFrame(const communication::SetExhibitFrameRequest &request) override;

    virtual void getExperimentData(communication::ExperimentData &response) override;

    virtual void createNewAction(communication::NewActionResponse &response,
                                 const communication::NewActionRequest &request) override;
    virtual void getActions(communication::ActionsResponse &response) override;

private:
    db::Database &db;
    apache::thrift::server::TServer *srv;

    std::mutex setMapLock;

    template <class F>
    typename std::result_of<F()>::type withExceptionTranslation(F &&f);
};

template <class F>
typename std::result_of<F()>::type CommandHandler::withExceptionTranslation(F &&f) {
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