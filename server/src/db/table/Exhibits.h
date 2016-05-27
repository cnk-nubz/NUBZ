#ifndef DB_TABLE__EXHIBITS__H
#define DB_TABLE__EXHIBITS__H

#include <cstdint>
#include <string>

#include <db/sql/sql.h>

#include "Column.h"
#include "Field.h"

namespace db {
namespace table {

struct Exhibits {
    struct FieldID : detail::Field<std::int32_t, Exhibits> {
        using detail::Field<std::int32_t, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldID> ID{};

    struct FieldName : detail::Field<std::string, Exhibits> {
        using detail::Field<std::string, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldName> Name{};

    struct FieldVersion : detail::Field<std::int32_t, Exhibits> {
        using detail::Field<std::int32_t, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldVersion> Version{};

    struct FieldRgbHex : detail::Field<std::int32_t, Exhibits> {
        using detail::Field<std::int32_t, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldRgbHex> RgbHex{};

    struct FieldFrameX : detail::OptField<std::int32_t, Exhibits> {
        using detail::OptField<std::int32_t, Exhibits>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFrameX> FrameX{};

    struct FieldFrameY : detail::OptField<std::int32_t, Exhibits> {
        using detail::OptField<std::int32_t, Exhibits>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFrameY> FrameY{};

    struct FieldFrameWidth : detail::OptField<std::int32_t, Exhibits> {
        using detail::OptField<std::int32_t, Exhibits>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFrameWidth> FrameWidth{};

    struct FieldFrameHeight : detail::OptField<std::int32_t, Exhibits> {
        using detail::OptField<std::int32_t, Exhibits>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFrameHeight> FrameHeight{};

    struct FieldFrameFloor : detail::OptField<std::int32_t, Exhibits> {
        using detail::OptField<std::int32_t, Exhibits>::OptField;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldFrameFloor> FrameFloor{};

    // default = false
    struct FieldIsDeleted : detail::Field<bool, Exhibits> {
        using detail::Field<bool, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldIsDeleted> IsDeleted{};

    // default = 0
    struct FieldRefCount : detail::Field<std::int32_t, Exhibits> {
        using detail::Field<std::int32_t, Exhibits>::Field;
        static const std::string columnName;
    };
    static constexpr detail::Column<FieldRefCount> RefCount{};

    static const std::string tableName;

    template <class... StandardFields>
    struct SqlCore {
        using in_t = std::tuple<StandardFields...>;
        using out_t = std::tuple<FieldID, FieldIsDeleted, StandardFields...>;

        static auto select() {
            return sql::Select<FieldID, FieldIsDeleted, StandardFields...>{};
        }

        static auto insert() {
            return sql::Insert<StandardFields...>{};
        }

        static auto update() {
            return sql::Update<FieldID, FieldIsDeleted, FieldRefCount, StandardFields...>{};
        }

        static auto del() {
            return sql::Delete<FieldID, FieldIsDeleted, FieldRefCount, StandardFields...>{};
        }
    };

    using Sql = SqlCore<FieldName, FieldVersion, FieldRgbHex, FieldFrameX, FieldFrameY,
                        FieldFrameWidth, FieldFrameHeight, FieldFrameFloor>;
};
}
}

#endif
