#ifndef COMMAND__GET_MAP_IMAGES_COMMAND__H
#define COMMAND__GET_MAP_IMAGES_COMMAND__H

#include "AbstractCommand.h"
#include "io/input/MapImagesRequest.h"
#include "io/output/MapImagesResponse.h"
#include "db/Database.h"

namespace command {
    class GetMapImagesCommand
        : public AbstractCommand<io::input::MapImagesRequest, io::output::MapImagesResponse> {
    public:
        static void setUrlPathPrefix(const std::string &urlPrefix);

        GetMapImagesCommand(db::Database &db);
        virtual ~GetMapImagesCommand() = default;

        GetMapImagesCommand(const GetMapImagesCommand &) = delete;
        GetMapImagesCommand(GetMapImagesCommand &&) = default;
        GetMapImagesCommand &operator=(const GetMapImagesCommand &) = delete;
        GetMapImagesCommand &operator=(GetMapImagesCommand &&) = default;

        virtual io::output::MapImagesResponse perform(
            const io::input::MapImagesRequest &input) override;

    private:
        static std::string urlPathPrefix;
        db::Database &db;

        std::string createFullUrl(const std::string &imgFileName) const;
    };
}

#endif