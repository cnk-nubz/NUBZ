#ifndef SERVER_COMMAND__MAP_COMMANDS__H
#define SERVER_COMMAND__MAP_COMMANDS__H

#include <atomic>
#include <cstdint>
#include <memory>

#include <boost/filesystem.hpp>

#include <utils/FileHandler.h>
#include <utils/ImageProcessor.h>

#include <db/Database.h>

#include <repository/MapImages.h>

#include <server/io/input/NewMapImagesRequest.h>
#include <server/io/input/SetMapImageRequest.h>
#include <server/io/output/MapImage.h>
#include <server/io/output/NewMapImagesResponse.h>

#include "Command.h"

namespace server {
namespace command {

using namespace io::input;
using namespace io::output;

class MapCommands : public Command {
public:
    MapCommands(db::Database &db);

    NewMapImagesResponse getNew(const NewMapImagesRequest &input);
    MapImage set(const SetMapImageRequest &input);

private:
    struct ZoomLevelInfo {
        std::int32_t levelIdx;
        std::size_t size;
        std::size_t tileSize;
    };
    static const std::vector<ZoomLevelInfo> zoomLevels;
    static volatile std::atomic_flag inProgress;

    void removeOldData(std::int32_t floor);
    void createImageProc(const std::string &tmpMapFilename);

    ::utils::FileHandler createFloorDirectory(std::int32_t floor);

    // creates and saves in public directory
    ::utils::FileHandler createFullSizeImage(const std::string &filename, std::int32_t *widthOut,
                                             std::int32_t *heightOut);

    // creates tiles like {dst}/{zoom_level}/{x}/{y}.jpg
    // doesn't handle any errors
    std::vector<repository::MapImage::ZoomLevel> createZoomLevels(
        const boost::filesystem::path &dst, const std::string &filenamePrefix);

    // creates tiles like: {dst}/{x}/{y}.jpg
    // doesn't handle any errors
    repository::MapImage::ZoomLevel createZoomLevelTiles(const ZoomLevelInfo &zoomLevelInfo,
                                                         const boost::filesystem::path &dst,
                                                         const std::string &filenamePrefix);

    void prepareImageProcessor(const ZoomLevelInfo &zoomLevel);

    std::unique_ptr<::utils::ImageProcessor> imgProc;
    db::Database &db;
};
}
}

#endif