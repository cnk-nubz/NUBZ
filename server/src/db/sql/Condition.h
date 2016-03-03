#ifndef DB_SQL__CONDITION__H
#define DB_SQL__CONDITION__H

#include <string>
#include <type_traits>

#include <boost/format.hpp>

#include <utils/type_traits.h>

#include "utils.h"

namespace db {
namespace sql {
namespace detail {

struct Op {
protected:
    Op() = default;
};

struct NullOp {
protected:
    NullOp() = default;
};

struct _EqualNull : NullOp {
    std::string str() const {
        return "IS NULL";
    }
} static constexpr IsNull{};

struct _NotEqualNull : NullOp {
    std::string str() const {
        return "IS NOT NULL";
    }
} static constexpr IsNotNull{};

struct _Equal : Op {
    std::string str() const {
        return "=";
    }
} static constexpr Equal{};

struct _NotEqual : Op {
    std::string str() const {
        return "<>";
    }
} static constexpr NotEqual{};

struct _Greater : Op {
    std::string str() const {
        return ">";
    }
} static constexpr Greater{};

struct _GreaterEqual : Op {
    std::string str() const {
        return ">=";
    }
} static constexpr GreaterEqual{};

struct _Less : Op {
    std::string str() const {
        return "<";
    }
} static constexpr Less{};

struct _LessEqual : Op {
    std::string str() const {
        return "<=";
    }
} static constexpr LessEqual{};

template <class... UsedColumns>
class Condition {
public:
    Condition(std::string stmt) : stmt(std::move(stmt)) {
    }

    template <class Column, class NullOperator>
    Condition(Column&&, NullOperator&& op) {
        check_operator_type<NullOp, NullOperator>();
        check_column<Column>();
        static_assert(std::decay_t<Column>::value_type::is_optional,
                      "not null column shouldn't be checked for null");

        stmt = createStmt(utils::getColumnName<Column>(), op.str());
    }

    template <class Column, class Operator, class Value>
    Condition(Column&&, Operator&& op, Value&& value) {
        check_operator_type<Op, Operator>();
        check_column<Column>();
        check_value_type<Column, Value>();

        stmt = createStmt(utils::getColumnName<Column>(), op.str(), std::forward<Value>(value));
    }

    template <class... UsedColumns2>
    auto operator&&(const Condition<UsedColumns2...>& cond) const {
        return Condition<UsedColumns..., UsedColumns2...>(
            "(" + createStmt(str(), "AND", cond.str()) + ")");
    }

    template <class... UsedColumns2>
    auto operator||(const Condition<UsedColumns2...>& cond) const {
        return Condition<UsedColumns..., UsedColumns2...>(
            "(" + createStmt(str(), "OR", cond.str()) + ")");
    }

    std::string str() const {
        return stmt;
    }

private:
    template <class Column>
    static constexpr void check_column() noexcept {
        static_assert(::utils::types::find_type<std::decay_t<Column>, UsedColumns...>::value,
                      "invalid column");
    }

    template <class Column, class Value>
    static constexpr void check_value_type() noexcept {
        static_assert(std::is_same<typename std::decay_t<Column>::value_type::internal_type,
                                   std::decay_t<Value>>::value,
                      "invalid value type");
    }

    template <class Base, class Operator>
    static constexpr void check_operator_type() noexcept {
        static_assert(std::is_base_of<Base, std::decay_t<Operator>>::value, "wrong operator type");
    }

    template <class V>
    std::string createStmt(std::string lhs, std::string op, V&& rhs) const {
        return (boost::format("%1% %2% %3%") % std::move(lhs) % std::move(op) %
                utils::sqlVal(std::forward<V>(rhs)))
            .str();
    }

    std::string createStmt(std::string lhs, std::string op) const {
        return (boost::format("%1% %2%") % std::move(lhs) % std::move(op)).str();
    }

    std::string stmt;
};
}
}
}

#endif