#ifndef COMMAND__SET_MAP_IMAGE_COMMAND__H
#define COMMAND__SET_MAP_IMAGE_COMMAND__H

#include "commands_common.h"
#include "io/input/SetMapImageRequest.h"
#include "db/Database.h"

namespace command {
    class SetMapImageCommand {
    public:
        SetMapImageCommand(db::Database &db);
        ~SetMapImageCommand() = default;

        DEFAULT_CP_MV(SetMapImageCommand);

        void operator()(const io::input::SetMapImageRequest &input);

    private:
        db::Database &db;

        std::int32_t getCurrentVersion(db::DatabaseSession &session) const;
        std::string createFilename(const std::string &srcFilename, std::int32_t level) const;
        void moveFileToPublic(const std::string &srcFilename,
                              const std::string &destFilename) const;
    };
}

#endif
