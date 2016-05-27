#ifndef DB_TABLE__SQL_CORE__H
#define DB_TABLE__SQL_CORE__H

#include <tuple>

#include <db/sql/sql.h>

namespace db {
namespace table {
namespace detail {

template <class... Fields>
struct SqlCore {
    using in_t = std::tuple<Fields...>;
    using out_t = std::tuple<Fields...>;

    static auto select() {
        return sql::Select<Fields...>{};
    }

    static auto insert() {
        return sql::Insert<Fields...>{};
    }

    static auto update() {
        return sql::Update<Fields...>{};
    }

    static auto del() {
        return sql::Delete<Fields...>{};
    }
};

template <class FieldID, class... Fields>
struct SqlCoreID {
    using in_t = std::tuple<Fields...>;
    using out_t = std::tuple<FieldID, Fields...>;

    static auto select() {
        return sql::Select<FieldID, Fields...>{};
    }

    static auto insert() {
        return sql::Insert<Fields...>{};
    }

    static auto update() {
        return sql::Update<FieldID, Fields...>{};
    }

    static auto del() {
        return sql::Delete<FieldID, Fields...>{};
    }
};

template <class FieldID, class FieldRefCount, class... Fields>
struct SqlCoreIDRefCount {
    using in_t = std::tuple<Fields...>;
    using out_t = std::tuple<FieldID, FieldRefCount, Fields...>;

    static auto select() {
        return sql::Select<FieldID, FieldRefCount, Fields...>{};
    }

    static auto insert() {
        return sql::Insert<Fields...>{};
    }

    static auto update() {
        return sql::Update<FieldID, FieldRefCount, Fields...>{};
    }

    static auto del() {
        return sql::Delete<FieldID, FieldRefCount, Fields...>{};
    }
};
}
}
}

#endif