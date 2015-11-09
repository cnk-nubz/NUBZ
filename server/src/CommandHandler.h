#ifndef COMMAND_HANDLER__H
#define COMMAND_HANDLER__H

#include <thrift/server/TServer.h>

#include "communication/Server.h"

class CommandHandler : public communication::ServerIf {
public:
    CommandHandler();
    
    void setServer(apache::thrift::server::TServer *srv);
    
    virtual void shutdown() override;
    virtual std::int32_t ping(const communication::HelloMsg &msg) override;
    
private:
    apache::thrift::server::TServer *srv;
};

#endif