#include "Exhibits.h"

namespace db {
namespace table {

const std::string Exhibits::tableName = "exhibits";

const std::string Exhibits::FieldID::columnName = "id";
const std::string Exhibits::FieldName::columnName = "name";
const std::string Exhibits::FieldVersion::columnName = "version";

const std::string Exhibits::FieldFrameX::columnName = "map_frame_x";
const std::string Exhibits::FieldFrameY::columnName = "map_frame_y";
const std::string Exhibits::FieldFrameWidth::columnName = "map_frame_width";
const std::string Exhibits::FieldFrameHeight::columnName = "map_frame_height";
const std::string Exhibits::FieldFrameFloor::columnName = "map_floor";
}
}