#ifndef COMMAND_HANDLER__H
#define COMMAND_HANDLER__H

#include <thrift/server/TServer.h>

#include "communication/Server.h"
#include "db/Database.h"

class CommandHandler : public communication::ServerIf {
public:
    CommandHandler(db::Database &db);

    void setServer(apache::thrift::server::TServer *srv);

    virtual void shutdown() override;
    virtual std::int32_t ping(const communication::HelloMsg &msg) override;

    virtual void getMapImages(communication::MapImagesResponse &response,
                              const communication::MapImagesRequest &request) override;
    virtual void setMapImage(const communication::SetMapImageRequest &request) override;

private:
    db::Database &db;
    apache::thrift::server::TServer *srv;
};

#endif