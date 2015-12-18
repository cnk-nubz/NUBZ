#include <boost/filesystem.hpp>
#include <Magick++.h>

#include "external/easylogging++.h"

#include "SetMapImageCommand.h"
#include "InvalidInput.h"
#include "db/command/GetMapImages.h"
#include "db/command/GetCounter.h"
#include "db/command/SetCounter.h"
#include "db/command/SaveMapImage.h"
#include "db/command/GetMapTiles.h"
#include "db/command/RemoveMapTiles.h"
#include "db/command/RemoveMapTilesInfo.h"
#include "db/command/SaveMapTile.h"
#include "db/command/SaveMapTilesInfo.h"
#include "FileHelper.h"
#include "image/ImageProcessor.h"

namespace command {
    SetMapImageCommand::SetMapImageCommand(db::Database &db) : db(db) {
    }

    void SetMapImageCommand::operator()(const io::input::SetMapImageRequest &input) {
        randVal = rand() % 1000;
        tiles.clear();
        tilesInfo.clear();

        const auto pathToImg = FileHelper::getInstance().pathForTmpFile(input.filename);
        std::string publicFilename = createFilename(input.filename, input.level);
        auto tilesHandlers = createTiles(pathToImg.string(), input.level);

        std::vector<db::MapTile> oldTiles;

        LOG(INFO) << "Saving information about image and tiles to database";
        db.execute([&](db::DatabaseSession &session) {
            // get old images
            {
                db::cmd::GetMapTiles getMapTiles(input.level);
                getMapTiles(session);
                oldTiles = getMapTiles.getResult();

                // todo
                // get current map image and remove it after update
            }

            // get version number
            std::int32_t nextVersion = getCurrentVersion(session) + 1;
            db::cmd::SetCounter(db::info::counters::element_type::map_images, nextVersion)(session);

            // save image
            db::MapImage mapImage;
            mapImage.floor = input.level;
            mapImage.filename = publicFilename;
            mapImage.version = nextVersion;
            setSize(mapImage, pathToImg.string());
            db::cmd::SaveMapImage{mapImage}(session);

            // remove old and insert new tiles
            db::cmd::RemoveMapTiles{input.level}(session);
            for (const auto &tile : tiles) {
                db::cmd::SaveMapTile{tile}(session);
            }

            // remove old and insert new tiles info
            db::cmd::RemoveMapTilesInfo{input.level}(session);
            for (const auto &tileInfo : tilesInfo) {
                db::cmd::SaveMapTilesInfo{tileInfo}(session);
            }

            moveFileToPublic(input.filename, publicFilename);
        });

        for (auto &handler : tilesHandlers) {
            handler.release();
        }
        for (const auto &toDelete : oldTiles) {
            boost::system::error_code ec;
            boost::filesystem::remove(
                FileHelper::getInstance().pathForPublicFile(toDelete.filename), ec);
        }
    }

    std::vector<utils::FileHandler> SetMapImageCommand::createTiles(const std::string &filePath,
                                                                    std::int32_t floor) {
        static const std::size_t tileSize = 256;
        std::vector<utils::FileHandler> handlers;

        img::ImageProcessor imgProc(filePath);
        imgProc.setTileSize(tileSize);

        db::MapTile mapTile;
        mapTile.floor = floor;

        db::MapTilesInfo mapTilesInfo;
        mapTilesInfo.floor = floor;
        mapTilesInfo.tileSize = tileSize;

        std::vector<std::pair<std::int32_t, std::size_t>> sizes = {{1, 2048}, {2, 4096}, {3, 8192}};
        for (const auto &levelAndSize : sizes) {
            LOG(INFO) << "Generating tiles for level " << levelAndSize.first;

            std::string pathPrefix =
                FileHelper::getInstance().pathPrefixForImageTile(floor, levelAndSize.first) + "_r" +
                std::to_string(randVal);
            imgProc.setScale(levelAndSize.second);

            std::size_t widthAfterScale;
            std::size_t heightAfterScale;
            const auto tilesFilenames =
                imgProc.generateTiles(pathPrefix, &widthAfterScale, &heightAfterScale);

            mapTile.zoomLevel = levelAndSize.first;
            mapTilesInfo.imgWidth = (std::int32_t)widthAfterScale;
            mapTilesInfo.imgHeight = (std::int32_t)heightAfterScale;
            mapTilesInfo.zoomLevel = levelAndSize.first;
            mapTilesInfo.colsCount = (std::int32_t)tilesFilenames.back().size();
            mapTilesInfo.rowsCount = (std::int32_t)tilesFilenames.size();

            for (std::size_t rowIdx = 0; rowIdx < tilesFilenames.size(); rowIdx++) {
                const auto &row = tilesFilenames[rowIdx];
                for (std::size_t colIdx = 0; colIdx < row.size(); colIdx++) {
                    const auto &col = row[colIdx];

                    handlers.emplace_back(col);

                    mapTile.row = (std::int32_t)rowIdx;
                    mapTile.col = (std::int32_t)colIdx;
                    mapTile.filename = getFilename(handlers.back().getPath());
                    tiles.push_back(mapTile);
                }
            }
            tilesInfo.push_back(mapTilesInfo);
        }

        return handlers;
    }

    std::int32_t SetMapImageCommand::getCurrentVersion(db::DatabaseSession &session) const {
        db::cmd::GetCounter cmd(db::info::counters::element_type::map_images);
        cmd(session);
        return cmd.getResult();
    }

    std::string SetMapImageCommand::createFilename(const std::string &srcFilename,
                                                   std::int32_t level) const {
        boost::filesystem::path src = srcFilename;
        boost::filesystem::path dst =
            "map_l" + std::to_string(level) + "_" + std::to_string(randVal);
        if (src.has_extension()) {
            dst.replace_extension(src.extension());
        }

        return dst.filename().native();
    }

    void SetMapImageCommand::setSize(db::MapImage &mapImage, const std::string imgPath) const {
        Magick::Image img(imgPath);
        mapImage.width = static_cast<decltype(mapImage.width)>(img.columns());
        mapImage.height = static_cast<decltype(mapImage.height)>(img.rows());
    }

    std::string SetMapImageCommand::getFilename(const std::string &path) const {
        return boost::filesystem::path(path).filename().string();
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