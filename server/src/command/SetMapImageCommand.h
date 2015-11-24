#ifndef COMMAND__SET_MAP_IMAGE_COMMAND__H
#define COMMAND__SET_MAP_IMAGE_COMMAND__H

#include "AbstractCommand.h"
#include "io/input/SetMapImageRequest.h"
#include "db/Database.h"

namespace command {
    class SetMapImageCommand : public AbstractCommandOnlyIn<io::input::SetMapImageRequest> {
    public:
        SetMapImageCommand(db::Database &db);
        virtual ~SetMapImageCommand() = default;

        SetMapImageCommand(const SetMapImageCommand &) = delete;
        SetMapImageCommand(SetMapImageCommand &&) = default;
        SetMapImageCommand &operator=(const SetMapImageCommand &) = delete;
        SetMapImageCommand &operator=(SetMapImageCommand &&) = default;

        virtual void perform(const io::input::SetMapImageRequest &input) override;

    private:
        db::Database &db;

        std::int32_t getCurrentVersion(db::DatabaseSession &session) const;
        std::string createFilename(const std::string &srcFilename, std::int32_t level) const;
        void moveFileToPublic(const std::string &srcFilename,
                              const std::string &destFilename) const;
    };
}

#endif
