#ifndef DB_TABLE__COLUMN__H
#define DB_TABLE__COLUMN__H

#include <string>

#include <db/sql/Condition.h>

namespace db {
namespace table {

struct _Null {
} static constexpr Null{};

namespace detail {

template <class Field>
struct Column2 {
    using field_type = Field;
};

template <class Field, class Value = typename Field::internal_type>
auto operator<(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Less, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator<=(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, LessEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator>(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Greater, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator>=(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, GreaterEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator==(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, Equal, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator!=(Column2<Field> col, const Value &val) {
    using namespace db::sql::detail;
    return Condition<Field>(col, NotEqual, val);
}

template <class Field, class Value = typename Field::internal_type>
auto operator==(Column2<Field> col, _Null) {
    using namespace db::sql::detail;
    return Condition<Field>(col, IsNull);
}

template <class Field, class Value = typename Field::internal_type>
auto operator!=(Column2<Field> col, _Null) {
    using namespace db::sql::detail;
    return Condition<Field>(col, IsNotNull);
}

template <class Subclass, class Table, class ValueType>
struct Column {
    using table_type = Table;
    using value_type = ValueType;
    using self_type = Subclass;

    auto operator<(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, Less, val);
    }

    auto operator<=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, LessEqual, val);
    }

    auto operator>(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, Greater, val);
    }

    auto operator>=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, GreaterEqual, val);
    }

    auto operator==(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, Equal, val);
    }

    auto operator!=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, NotEqual, val);
    }

    auto operator==(const _Null &) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, IsNull);
    }

    auto operator!=(const _Null &) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*this, IsNotNull);
    }

    const self_type *self() const {
        return static_cast<const self_type *>(this);
    }
};
}
}
}

#endif