#ifndef DB_TABLE__FIELD__H
#define DB_TABLE__FIELD__H

#include <db/sql/translation.h>

namespace db {
namespace table {
namespace detail {

template <class T, class Table>
struct Field {
    using type = T;
    using internal_type = T;
    using table_type = Table;
    static constexpr bool is_optional = false;

    explicit Field(type v) : value(std::move(v)) {
    }
    const type value;
};

template <class T, class Table>
struct OptField {
    using type = boost::optional<T>;
    using internal_type = T;
    using table_type = Table;
    static constexpr bool is_optional = true;

    OptField() = default;
    explicit OptField(type v) : value(std::move(v)) {
    }
    const type value;
};

template <class Field, bool optional = Field::is_optional>
struct FieldFactory;

template <class Field>
struct FieldFactory<Field, true> {
    static Field create(const boost::optional<std::string> &raw) {
        return !raw ? Field{{}} : FieldFactory<Field, false>::create(raw.value());
    };
};

template <class Field>
struct FieldFactory<Field, false> {
    static Field create(const boost::optional<std::string> &raw) {
        assert(raw);
        return create(raw.value());
    };

    static Field create(const std::string &raw) {
        return Field{sql::translation::sql_val_cast<typename Field::internal_type>::from(raw)};
    };
};
}
}
}

#endif