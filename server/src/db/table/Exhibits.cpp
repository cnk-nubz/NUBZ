#include "Exhibits.h"

namespace db {
namespace table {

const std::string Exhibits::tableName = "exhibits";
const std::string Exhibits::ColumnId::name = "id";
const std::string Exhibits::ColumnName::name = "name";
const std::string Exhibits::ColumnVersion::name = "version";

const std::string Exhibits::ColumnFrameX::name = "map_frame_x";
const std::string Exhibits::ColumnFrameY::name = "map_frame_y";
const std::string Exhibits::ColumnFrameWidth::name = "map_frame_width";
const std::string Exhibits::ColumnFrameHeight::name = "map_frame_height";
const std::string Exhibits::ColumnFrameFloor::name = "map_floor";

const Exhibits::ColumnId Exhibits::colId{};
const Exhibits::ColumnName Exhibits::colName{};
const Exhibits::ColumnVersion Exhibits::colVersion{};

const Exhibits::ColumnFrameX Exhibits::colFrameX{};
const Exhibits::ColumnFrameY Exhibits::colFrameY{};
const Exhibits::ColumnFrameWidth Exhibits::colFrameWidth{};
const Exhibits::ColumnFrameHeight Exhibits::colFrameHeight{};
const Exhibits::ColumnFrameFloor Exhibits::colFrameFloor{};

Exhibits::Row Exhibits::RowFactory::fromDB(const DBOut &dbOut) {
    auto res = Exhibits::Row{};
    res.ID = std::get<ValueId>(dbOut).value;
    res.name = std::get<ValueName>(dbOut).value;
    res.version = std::get<ValueVersion>(dbOut).value;

    res.frameX = std::get<ValueFrameX>(dbOut).value;
    res.frameY = std::get<ValueFrameY>(dbOut).value;
    res.frameWidth = std::get<ValueFrameWidth>(dbOut).value;
    res.frameHeight = std::get<ValueFrameHeight>(dbOut).value;
    res.frameFloor = std::get<ValueFrameFloor>(dbOut).value;

    return res;
}

Exhibits::RowFactory::DBIn Exhibits::RowFactory::toDB(const Exhibits::Row &row) {
    return std::make_tuple(ValueName{row.name},
                           ValueVersion{row.version},
                           ValueFrameX{row.frameX},
                           ValueFrameY{row.frameY},
                           ValueFrameWidth{row.frameWidth},
                           ValueFrameHeight{row.frameHeight},
                           ValueFrameFloor{row.frameFloor});
}
}
}