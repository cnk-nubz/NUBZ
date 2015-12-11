#ifndef COMMAND__GET_EXHIBITS_COMMAND__H
#define COMMAND__GET_EXHIBITS_COMMAND__H

#include "commands_common.h"
#include "io/input/ExhibitsRequest.h"
#include "io/output/ExhibitsResponse.h"
#include "db/Database.h"

namespace command {
    class GetExhibitsCommand {
    public:
        GetExhibitsCommand(db::Database &db);
        ~GetExhibitsCommand() = default;

        DEFAULT_CP_MV(GetExhibitsCommand);

        io::output::ExhibitsResponse operator()(const io::input::ExhibitsRequest &input);

    private:
        db::Database &db;
    };
}

#endif