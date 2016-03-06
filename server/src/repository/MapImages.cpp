#include <utils/fp_algorithm.h>

#include <db/table/MapImages.h>

#include "DefaultRepo.h"
#include "MapImages.h"

namespace repository {
using Table = db::table::MapImages;
using Impl = detail::DefaultRepo<Table>;

namespace {
Table::Row toDB(const MapImages::MapImage &mapImage);
MapImages::MapImage fromDB(const Table::Row &mapImage);
}

MapImages::MapImages(db::DatabaseSession &session) : session(session) {
}

boost::optional<MapImages::MapImage> MapImages::get(std::int32_t floor) {
    auto select = Table::select().where(Table::colFloor == floor);
    if (auto row = session.getResult(select)) {
        return fromDB(Table::RowFactory::fromDB(row.value()));
    } else {
        return {};
    }
}

std::vector<MapImage> MapImages::getAllNewerThan(std::int32_t version) {
    auto sql = Table::select().where(Table::colVersion > version);
    auto dbTuples = session.getResults(sql);

    auto result = std::vector<MapImage>{};
    ::utils::transform(
        dbTuples, result, [](auto &dbTuple) { return fromDB(Table::RowFactory::fromDB(dbTuple)); });
    return result;
}

std::vector<MapImages::MapImage> MapImages::getAll() {
    auto dbRows = Impl::getAll(session);
    auto result = std::vector<MapImages::MapImage>{};
    ::utils::transform(dbRows, result, fromDB);
    return result;
}

void MapImages::removeAll() {
    Impl::removeAll(session);
}

void MapImages::set(const MapImage &mapImage) {
    auto select = Table::select().where(Table::colFloor == mapImage.floor);
    if (session.getResult(select)) {
        remove(mapImage.floor);
    }

    Impl::insert(session, toDB(mapImage));
}

void MapImages::remove(std::int32_t floor) {
    auto sql = Table::del().where(Table::colFloor == floor);
    session.execute(sql);
}

namespace {
Table::Row toDB(const MapImages::MapImage &mapImage) {
    auto res = Table::Row{};
    res.floor = mapImage.floor;
    res.width = mapImage.width;
    res.height = mapImage.height;
    res.version = mapImage.version;
    res.filename = mapImage.filename;
    for (auto &zoomLevel : mapImage.zoomLevels) {
        auto resZoomLevel = Table::Row::ZoomLevel{};
        resZoomLevel.imageWidth = zoomLevel.imageWidth;
        resZoomLevel.imageHeight = zoomLevel.imageHeight;
        resZoomLevel.tileSize = zoomLevel.tileSize;
        resZoomLevel.tilesFilenames = zoomLevel.tilesFilenames;
        res.zoomLevels.push_back(resZoomLevel);
    }
    return res;
}

MapImages::MapImage fromDB(const Table::Row &mapImage) {
    auto res = MapImage{};
    res.floor = mapImage.floor;
    res.width = mapImage.width;
    res.height = mapImage.height;
    res.version = mapImage.version;
    res.filename = mapImage.filename;
    for (auto &zoomLevel : mapImage.zoomLevels) {
        auto resZoomLevel = MapImage::ZoomLevel{};
        resZoomLevel.imageWidth = zoomLevel.imageWidth;
        resZoomLevel.imageHeight = zoomLevel.imageHeight;
        resZoomLevel.tileSize = zoomLevel.tileSize;
        resZoomLevel.tilesFilenames = zoomLevel.tilesFilenames;
        res.zoomLevels.push_back(resZoomLevel);
    }
    return res;
}
}
}