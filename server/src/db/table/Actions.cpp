#include "Actions.h"

namespace db {
namespace table {

const std::string Actions::tableName = "actions";
const std::string Actions::ColumnId::name = "id";
const std::string Actions::ColumnText::name = "text";

const Actions::ColumnId Actions::colId{};
const Actions::ColumnText Actions::colText{};

Actions::Row Actions::RowFactory::fromDB(const DBOut &dbOut) {
    Actions::Row res;
    res.ID = std::get<ValueId>(dbOut).value;
    res.text = std::get<ValueText>(dbOut).value;
    return res;
}

Actions::RowFactory::DBIn Actions::RowFactory::toDB(const db::table::Actions::Row &row) {
    return std::make_tuple(ValueText{row.text});
}
}
}