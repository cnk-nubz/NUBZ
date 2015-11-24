#include <boost/filesystem.hpp>

#include "SetMapImageCommand.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetVersion.h"
#include "db/command/SetVersion.h"
#include "db/command/SaveMapImage.h"
#include "FileHelper.h"

namespace command {
    SetMapImageCommand::SetMapImageCommand(db::Database &db) : db(db) {
    }

    void SetMapImageCommand::perform(const io::input::SetMapImageRequest &input) {
        db.execute([&](db::DatabaseSession &session) {
            std::int32_t nextVersion = getCurrentVersion(session) + 1;
            db::cmd::SetVersion(db::info::versions::element_type::map_images, nextVersion)(session);

            std::string publicFilename = createFilename(input.filename, input.level);

            db::MapImage mapImage;
            mapImage.level = input.level;
            mapImage.filename = publicFilename;
            mapImage.version = nextVersion;
            db::cmd::SaveMapImage{mapImage}(session);

            moveFileToPublic(input.filename, publicFilename);
        });
    }

    std::int32_t SetMapImageCommand::getCurrentVersion(db::DatabaseSession &session) const {
        db::cmd::GetVersion cmd(db::info::versions::element_type::map_images);
        cmd(session);
        return cmd.getResult();
    }

    std::string SetMapImageCommand::createFilename(const std::string &srcFilename,
                                                   std::int32_t level) const {
        boost::filesystem::path src = srcFilename;
        boost::filesystem::path dst =
            "map_l" + std::to_string(level) + "_" + std::to_string(rand() % 1000);
        if (src.has_extension()) {
            dst.replace_extension(src.extension());
        }

        return dst.filename().native();
    }

    void SetMapImageCommand::moveFileToPublic(const std::string &srcFilename,
                                              const std::string &destFilename) const {
        namespace fs = boost::filesystem;

        fs::path src = FileHelper::getInstance().pathForTmpFile(srcFilename);
        fs::path dst = FileHelper::getInstance().pathForPublicFile(destFilename);

        try {
            fs::rename(src, dst);
        } catch (std::runtime_error &e) {
            throw InvalidInput(e.what());
        }
    }
}