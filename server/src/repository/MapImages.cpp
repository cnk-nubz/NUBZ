#include <utils/fp_algorithm.h>

#include <db/table/MapImages.h>

#include "DefaultRepo.h"
#include "MapImages.h"

namespace repository {
using Table = db::table::MapImages;
using Impl = detail::DefaultRepo<Table>;

namespace {
Table::Sql::in_t toDB(const MapImages::MapImage &mapImage);
Table::ZoomLevelsData::ZoomLevel zoomLevelToDB(const MapImages::MapImage::ZoomLevel &zoomLevel);

MapImages::MapImage fromDB(const Table::Sql::out_t &mapImage);
MapImages::MapImage::ZoomLevel zoomLevelFromDB(const Table::ZoomLevelsData::ZoomLevel &zoomLevel);
}

MapImages::MapImages(db::DatabaseSession &session) : session(session) {
}

boost::optional<MapImages::MapImage> MapImages::get(std::int32_t floor) {
    auto sql = Table::Sql::select().where(Table::Floor == floor);
    if (auto row = session.getResult(sql)) {
        return fromDB(row.value());
    } else {
        return {};
    }
}

std::vector<MapImages::MapImage> MapImages::getAll() {
    auto dbRows = Impl::getAll(session);
    auto result = std::vector<MapImages::MapImage>{};
    utils::transform(dbRows, result, fromDB);
    return result;
}

void MapImages::removeAll() {
    Impl::removeAll(session);
}

void MapImages::set(const MapImage &mapImage) {
    remove(mapImage.floor);
    Impl::insert(session, toDB(mapImage));
}

void MapImages::remove(std::int32_t floor) {
    auto sql = Table::Sql::del().where(Table::Floor == floor);
    session.execute(sql);
}

namespace {
Table::Sql::in_t toDB(const MapImages::MapImage &mapImage) {
    auto zoomLevelsData = Table::ZoomLevelsData{};
    utils::transform(mapImage.zoomLevels, zoomLevelsData.zoomLevels, zoomLevelToDB);

    return std::make_tuple(Table::FieldFloor{mapImage.floor},
                           Table::FieldFilename{mapImage.filename},
                           Table::FieldWidth{mapImage.width},
                           Table::FieldHeight{mapImage.height},
                           Table::FieldZoomLevels{zoomLevelsData});
}

Table::ZoomLevelsData::ZoomLevel zoomLevelToDB(const MapImages::MapImage::ZoomLevel &zoomLevel) {
    auto res = Table::ZoomLevelsData::ZoomLevel{};
    res.imageWidth = zoomLevel.imageWidth;
    res.imageHeight = zoomLevel.imageHeight;
    res.tileSize = zoomLevel.tileSize;
    res.tilesFilenames = zoomLevel.tilesFilenames;
    return res;
}

MapImages::MapImage fromDB(const Table::Sql::out_t &mapImage) {
    auto res = MapImages::MapImage{};
    auto dbZoomLevels = std::get<Table::FieldZoomLevels>(mapImage).value.zoomLevels;
    utils::transform(dbZoomLevels, res.zoomLevels, zoomLevelFromDB);
    res.floor = std::get<Table::FieldFloor>(mapImage).value;
    res.filename = std::get<Table::FieldFilename>(mapImage).value;
    res.width = std::get<Table::FieldWidth>(mapImage).value;
    res.height = std::get<Table::FieldHeight>(mapImage).value;
    return res;
}

MapImages::MapImage::ZoomLevel zoomLevelFromDB(const Table::ZoomLevelsData::ZoomLevel &zoomLevel) {
    auto res = MapImages::MapImage::ZoomLevel{};
    res.imageWidth = zoomLevel.imageWidth;
    res.imageHeight = zoomLevel.imageHeight;
    res.tileSize = zoomLevel.tileSize;
    res.tilesFilenames = zoomLevel.tilesFilenames;
    return res;
}
}
}