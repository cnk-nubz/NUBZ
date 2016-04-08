#include <Magick++.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <utils/ImageProcessor.h>
#include <utils/log.h>

#include <repository/Counters.h>
#include <repository/Exhibits.h>
#include <repository/MapImages.h>

#include <server/io/InvalidInput.h>
#include <server/utils/PathHelper.h>

#include "MapCommands.h"

namespace server {
namespace command {

const std::vector<MapCommands::ZoomLevelInfo> MapCommands::zoomLevels = {
    {1, 1024, 64}, {2, 2048, 128}, {3, 4096, 256}, {4, 8192, 512}};
volatile std::atomic_flag MapCommands::inProgress = ATOMIC_FLAG_INIT;

MapCommands::MapCommands(db::Database &db) : db(db) {
}

std::map<std::int32_t, MapImage> MapCommands::getAll() {
    auto mapImages = db.execute(
        [&](db::DatabaseSession &session) { return repository::MapImages{session}.getAll(); });

    auto result = std::map<std::int32_t, MapImage>{};
    for (const auto &mapImage : mapImages) {
        result[mapImage.floor] = MapImage{mapImage};
    }
    return result;
}

void MapCommands::remove(std::int32_t floor) {
    if (inProgress.test_and_set()) {
        throw io::InvalidInput{"cannot change two maps at the same time"};
    }

    try {
        removeOldData(floor);
    } catch (...) {
        inProgress.clear();
        throw;
    }
    inProgress.clear();
}

MapImage MapCommands::set(const SetMapImageRequest &input) {
    if (inProgress.test_and_set()) {
        throw io::InvalidInput{"cannot change two maps at the same time"};
    }

    auto mapImage = repository::MapImage{};
    try {
        LOG(INFO) << "Removing current images";
        removeOldData(input.floor);

        auto imageDirHandler = createFloorDirectory(input.floor);
        createImageProc(input.filename);

        mapImage.floor = input.floor;

        LOG(INFO) << "Creating full size image";
        mapImage.filename = (boost::format("map_f%1%.jpg") % mapImage.floor).str();
        auto fullImageHandler =
            createFullSizeImage(mapImage.filename, &mapImage.width, &mapImage.height);

        LOG(INFO) << "Creating zoom levels";
        mapImage.zoomLevels =
            createZoomLevels(imageDirHandler.getPath(), std::to_string(mapImage.floor) + "/");

        LOG(INFO) << "Removing tmp data";
        boost::filesystem::remove(utils::PathHelper::tmpDir.pathForFile(input.filename));
        imgProc.reset();

        LOG(INFO) << "Saving data into database";
        db.execute(
            [&](db::DatabaseSession &session) { repository::MapImages{session}.set(mapImage); });

        fullImageHandler.release();
        imageDirHandler.release();
    } catch (...) {
        inProgress.clear();
        throw;
    }
    inProgress.clear();

    return MapImage{mapImage};
}

void MapCommands::removeOldData(std::int32_t floor) {
    boost::optional<repository::MapImage> mapImage = db.execute([&](db::DatabaseSession &session) {
        auto countersRepo = repository::Counters{session};
        auto exhibitsVersion = countersRepo.increment(repository::CounterType::LastExhibitVersion);
        repository::Exhibits{session}.resetFrames(floor, exhibitsVersion);

        auto repo = repository::MapImages{session};
        auto mapImage = repo.get(floor);
        if (mapImage) {
            repo.remove(floor);
        }
        return mapImage;
    });

    if (mapImage) {
        boost::filesystem::remove(
            utils::PathHelper::publicDir.pathForFile(mapImage.value().filename));
    }
    boost::filesystem::remove_all(utils::PathHelper::pathForFloorTilesDirectory(floor));
}

::utils::FileHandler MapCommands::createFloorDirectory(std::int32_t floor) {
    auto path = utils::PathHelper::pathForFloorTilesDirectory(floor);
    auto handler = ::utils::FileHandler{path};
    boost::filesystem::create_directory(path);
    return handler;
}

void MapCommands::createImageProc(const std::string &tmpMapFilename) {
    auto path = utils::PathHelper::tmpDir.pathForFile(tmpMapFilename);
    imgProc.reset(new ::utils::ImageProcessor{path});
}

::utils::FileHandler MapCommands::createFullSizeImage(const std::string &filename,
                                                      std::int32_t *widthOut,
                                                      std::int32_t *heightOut) {
    auto dst = utils::PathHelper::publicDir.pathForFile(filename);
    prepareImageProcessor(zoomLevels.back());
    auto handler = ::utils::FileHandler{dst};
    LOG(INFO) << "Saving file into " << dst;
    imgProc->saveImageTo(dst.string());

    *widthOut = (std::int32_t)imgProc->width();
    *heightOut = (std::int32_t)imgProc->height();

    return handler;
}

std::vector<repository::MapImage::ZoomLevel> MapCommands::createZoomLevels(
    const boost::filesystem::path &dst, const std::string &filenamePrefix) {
    auto result = std::vector<repository::MapImage::ZoomLevel>{};
    for (const auto &zoomLevelInfo : zoomLevels) {
        auto dirName = std::to_string(zoomLevelInfo.levelIdx);
        auto levelDst = dst / dirName;
        auto levelPrefix = filenamePrefix + dirName + "/";

        boost::filesystem::create_directory(levelDst);
        result.push_back(createZoomLevelTiles(zoomLevelInfo, levelDst, levelPrefix));
    }
    return result;
}

repository::MapImage::ZoomLevel MapCommands::createZoomLevelTiles(
    const ZoomLevelInfo &zoomLevelInfo, const boost::filesystem::path &dst,
    const std::string &filenamePrefix) {
    LOG(INFO) << "Creating image for zoom level " << zoomLevelInfo.levelIdx;
    prepareImageProcessor(zoomLevelInfo);

    auto fullImgPath = dst / "full.jpg";
    LOG(INFO) << "Saving file into " << fullImgPath;
    imgProc->saveImageTo(fullImgPath.string());

    LOG(INFO) << "Creating tiles for zoom level " << zoomLevelInfo.levelIdx << " in " << dst;
    auto tiles = imgProc->generateTiles(dst);

    // save data into repository struct
    auto zoomLevel = repository::MapImage::ZoomLevel{};
    zoomLevel.tileSize = (std::int32_t)zoomLevelInfo.tileSize;
    zoomLevel.imageWidth = (std::int32_t)imgProc->width();
    zoomLevel.imageHeight = (std::int32_t)imgProc->height();
    zoomLevel.tilesFilenames = tiles;

    // add prefix to each filename
    for (auto &row : zoomLevel.tilesFilenames) {
        for (auto &filename : row) {
            filename = filenamePrefix + filename;
        }
    }

    return zoomLevel;
}

void MapCommands::prepareImageProcessor(const ZoomLevelInfo &zoomLevel) {
    imgProc->reset();
    imgProc->scale(zoomLevel.size);
    imgProc->setTileSize(zoomLevel.tileSize);
    imgProc->addFrameToBeDivisibleBy(zoomLevel.tileSize);
}
}
}