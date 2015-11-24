#ifndef COMMAND__GET_EXHIBITS_COMMAND__H
#define COMMAND__GET_EXHIBITS_COMMAND__H

#include "AbstractCommand.h"
#include "io/input/ExhibitsRequest.h"
#include "io/output/ExhibitsResponse.h"
#include "db/Database.h"

namespace command {
    class GetExhibitsCommand
        : public AbstractCommand<io::input::ExhibitsRequest, io::output::ExhibitsResponse> {
    public:
        GetExhibitsCommand(db::Database &db);
        virtual ~GetExhibitsCommand() = default;

        GetExhibitsCommand(const GetExhibitsCommand &) = delete;
        GetExhibitsCommand(GetExhibitsCommand &&) = default;
        GetExhibitsCommand &operator=(const GetExhibitsCommand &) = delete;
        GetExhibitsCommand &operator=(GetExhibitsCommand &&) = default;

        virtual io::output::ExhibitsResponse perform(
            const io::input::ExhibitsRequest &input) override;

    private:
        db::Database &db;
    };
}

#endif