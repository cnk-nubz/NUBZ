#include <db/json/json.h>

#include "MapImages.h"

/*
 * columns:
 * - floor
 * - filename
 * - width
 * - height
 * - zoom levels
 *
 * zoom levels = [level, level, level, ...]
 * level = {imageWidth, imageHeight, tileSize, [[tile_name, ...], ...]}
 * width, height, tileSize = int
 * tile_name = string
 */

namespace db {
namespace table {

const std::string MapImages::tableName = "map_images";

const std::string MapImages::FieldFloor::columnName = "floor";
const std::string MapImages::FieldFilename::columnName = "filename";
const std::string MapImages::FieldWidth::columnName = "width";
const std::string MapImages::FieldHeight::columnName = "height";
const std::string MapImages::FieldZoomLevels::columnName = "zoom_levels";

namespace {
struct Keys {
    static const char *width;
    static const char *height;
    static const char *tileSize;
    static const char *tiles;
};

const char *Keys::width = "width";
const char *Keys::height = "height";
const char *Keys::tileSize = "tile_size";
const char *Keys::tiles = "tiles";

MapImages::ZoomLevelsData::ZoomLevel parseZoomLevel(const rapidjson::Value &json);
rapidjson::Value createZoomLevel(rapidjson::Document::AllocatorType &allocator,
                                 const MapImages::ZoomLevelsData::ZoomLevel &zoomLevel);
}

using namespace db::json;

MapImages::ZoomLevelsData::ZoomLevelsData(const std::string &jsonStr) {
    auto json = parseJson(jsonStr);
    zoomLevels = parseArray(json, parseZoomLevel);
}

MapImages::ZoomLevelsData::operator std::string() const {
    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createArray(allocator, zoomLevels, createZoomLevel);
    return jsonToString(json);
}

namespace {
MapImages::ZoomLevelsData::ZoomLevel parseZoomLevel(const rapidjson::Value &json) {
    auto res = MapImages::ZoomLevelsData::ZoomLevel{};
    res.imageWidth = parseInt(getNode(json, Keys::width));
    res.imageHeight = parseInt(getNode(json, Keys::height));
    res.tileSize = parseInt(getNode(json, Keys::tileSize));
    res.tilesFilenames = parseArray(getNode(json, Keys::tiles), parseStringArray);
    return res;
}

rapidjson::Value createZoomLevel(rapidjson::Document::AllocatorType &allocator,
                                 const MapImages::ZoomLevelsData::ZoomLevel &zoomLevel) {
    auto tiles = createArray(allocator, zoomLevel.tilesFilenames, createStringArray);

    return createDictionary(allocator,
                            std::make_pair(Keys::width, createInt(zoomLevel.imageWidth)),
                            std::make_pair(Keys::height, createInt(zoomLevel.imageHeight)),
                            std::make_pair(Keys::tileSize, createInt(zoomLevel.tileSize)),
                            std::make_pair(Keys::tiles, std::move(tiles)));
}
}
}
}
