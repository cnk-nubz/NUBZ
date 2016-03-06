#include <utils/fp_algorithm.h>

#include <db/json/json.h>

#include "MapImages.h"

/*
 * columns:
 * - floor
 * - filename
 * - width
 * - height
 * - version
 * - zoom levels (json)
 *
 * zoom levels = [level, level, level, ...]
 * level = {imageWidth, imageHeight, tileSize, [[tile_name, ...], ...]}
 * width, height, tileSize = int
 * tile_name = string
 */

namespace db {
namespace table {

const std::string MapImages::tableName = "map_images";
const std::string MapImages::ColumnFloor::name = "floor";
const std::string MapImages::ColumnFilename::name = "filename";
const std::string MapImages::ColumnWidth::name = "width";
const std::string MapImages::ColumnHeight::name = "height";
const std::string MapImages::ColumnVersion::name = "version";
const std::string MapImages::ColumnZoomLevels::name = "zoom_levels";

namespace {
struct Keys {
    static const std::string width;
    static const std::string height;
    static const std::string tileSize;
    static const std::string tiles;
};

const std::string Keys::width = "width";
const std::string Keys::height = "height";
const std::string Keys::tileSize = "tile_size";
const std::string Keys::tiles = "tiles";
}

const MapImages::ColumnFloor MapImages::colFloor{};
const MapImages::ColumnFilename MapImages::colFilename{};
const MapImages::ColumnWidth MapImages::colWidth{};
const MapImages::ColumnHeight MapImages::colHeight{};
const MapImages::ColumnVersion MapImages::colVersion{};
const MapImages::ColumnZoomLevels MapImages::colZoomLevels{};

namespace {
std::vector<MapImages::Row::ZoomLevel> parseRoot(const std::string &jsonStr);
MapImages::Row::ZoomLevel parseZoomLevel(const rapidjson::Value &json);
}

MapImages::Row MapImages::RowFactory::fromDB(const DBOut &dbOut) {
    MapImages::Row res;
    res.floor = std::get<ValueFloor>(dbOut).value;
    res.filename = std::get<ValueFilename>(dbOut).value;
    res.width = std::get<ValueWidth>(dbOut).value;
    res.height = std::get<ValueHeight>(dbOut).value;
    res.version = std::get<ValueVersion>(dbOut).value;
    res.zoomLevels = parseRoot(std::get<ValueZoomLevels>(dbOut).value);
    return res;
}

namespace {
using namespace db::json;

std::vector<MapImages::Row::ZoomLevel> parseRoot(const std::string &jsonStr) {
    auto json = parseJson(jsonStr);
    return parseArray(json, parseZoomLevel);
}

MapImages::Row::ZoomLevel parseZoomLevel(const rapidjson::Value &json) {
    auto res = MapImages::Row::ZoomLevel{};
    res.imageWidth = parseInt(getNode(json, Keys::width));
    res.imageHeight = parseInt(getNode(json, Keys::height));
    res.tileSize = parseInt(getNode(json, Keys::tileSize));
    res.tilesFilenames = parseArray(getNode(json, Keys::tiles), parseStringArray);
    for (const auto &row : res.tilesFilenames) {
        if (row.size() != res.tilesFilenames.back().size()) {
            assert(false && "all rows should have the same length");
        }
    }
    return res;
}
}

namespace {
std::string createRoot(const MapImages::Row &row);
rapidjson::Value createZoomLevel(rapidjson::Document::AllocatorType &allocator,
                                 const MapImages::Row::ZoomLevel &zoomLevel);
}

MapImages::RowFactory::DBIn MapImages::RowFactory::toDB(const db::table::MapImages::Row &row) {
    return std::make_tuple(ValueFloor{row.floor},
                           ValueFilename{row.filename},
                           ValueWidth{row.width},
                           ValueHeight{row.height},
                           ValueVersion{row.version},
                           ValueZoomLevels{createRoot(row)});
}

namespace {
using namespace db::json;

std::string createRoot(const MapImages::Row &row) {
    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createArray(allocator, row.zoomLevels, createZoomLevel);
    return jsonToString(json);
}

rapidjson::Value createZoomLevel(rapidjson::Document::AllocatorType &allocator,
                                 const MapImages::Row::ZoomLevel &zoomLevel) {
    auto tiles = createArray(allocator, zoomLevel.tilesFilenames, createStringArray);

    return createDictionary(allocator,
                            std::make_pair(Keys::width, rapidjson::Value{zoomLevel.imageWidth}),
                            std::make_pair(Keys::height, rapidjson::Value{zoomLevel.imageHeight}),
                            std::make_pair(Keys::tileSize, rapidjson::Value{zoomLevel.tileSize}),
                            std::make_pair(Keys::tiles, std::move(tiles)));
}
}
}
}
