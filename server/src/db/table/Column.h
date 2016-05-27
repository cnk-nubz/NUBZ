#ifndef DB_TABLE__COLUMN__H
#define DB_TABLE__COLUMN__H

#include <string>

#include <db/sql/Condition.h>

namespace db {
namespace table {
namespace detail {

template <class Field>
struct Column {
    using field_type = Field;
};

template <class Field, class Value = typename Field::internal_type>
auto operator<(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Less, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator<=(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, LessEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator>(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Greater, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator>=(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, GreaterEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator==(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Equal, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator!=(Column<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, NotEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator==(Column<Field> col, sql::_Null) {
    using namespace db::sql::detail;
    return Condition<Field>(col, IsNull);
}

template <class Field, class Value = typename Field::internal_type>
auto operator!=(Column<Field> col, sql::_Null) {
    using namespace db::sql::detail;
    return Condition<Field>(col, IsNotNull);
}
}
}
}

#endif