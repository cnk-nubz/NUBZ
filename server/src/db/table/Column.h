#ifndef DB_TABLE__COLUMN__H
#define DB_TABLE__COLUMN__H

#include <string>

#include <db/sql/Condition.h>

namespace db {
namespace table {

struct _Null {
} static constexpr Null{};

namespace detail {

template <class Subclass, class Table, class ValueType>
struct Column {
    using table_type = Table;
    using value_type = ValueType;
    using self_type = Subclass;

    auto operator<(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), Less, val);
    }

    auto operator<=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), LessEqual, val);
    }

    auto operator>(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), Greater, val);
    }

    auto operator>=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), GreaterEqual, val);
    }

    auto operator==(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), Equal, val);
    }

    auto operator!=(const typename value_type::internal_type &val) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), NotEqual, val);
    }

    auto operator==(const _Null &) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), IsNull);
    }

    auto operator!=(const _Null &) const {
        using namespace db::sql::detail;
        return Condition<self_type>(*self(), IsNotNull);
    }

    const self_type *self() const {
        return static_cast<const self_type *>(this);
    }
};
}
}
}

#endif