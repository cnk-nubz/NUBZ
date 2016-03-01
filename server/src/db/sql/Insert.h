#ifndef DB_SQL__INSERT__H
#define DB_SQL__INSERT__H

#include <string>
#include <cassert>

#include <boost/format.hpp>

#include "utils.h"

namespace db {
namespace sql {

template <class Subclass, class... Cols>
class _Insert {
    template <class Subclass2, class... Cols2>
    friend class _Insert;

    static_assert(utils::same_table<Cols...>::value, "all columns should be from the same table");

public:
    template <class Subclass2, class... Cols2>
    _Insert(_Insert<Subclass2, Cols2...> that)
        : vals(std::move(that.vals)) {
        static_assert(std::is_same<std::tuple<Cols...>, std::tuple<Cols2...>>::value,
                      "two _Inserts should have the same set of columns");
    }

    _Insert() = default;

    template <class... Values>
    Subclass &values(const std::tuple<Values...> &vals) {
        static_assert(sizeof...(Cols) == sizeof...(Values), "incorrect number of values in tuple");
        static_assert(
            std::is_same<std::tuple<typename Cols::value_type...>, std::tuple<Values...>>::value,
            "incorrect values types");

        auto strVals = tupleToStrings(vals, std::index_sequence_for<Values...>());
        this->vals.push_back("(" + utils::asSqlList(strVals) + ")");
        return *static_cast<Subclass *>(this);
    }

protected:
    std::string strPrefix() const {
        assert(vals.size() > 0 && "you should call .values() at least once");
        boost::format stmt("INSERT INTO %1%\n(%2%)\nVALUES\n%3%");
        stmt % utils::getTableName<Cols...>();
        stmt % utils::asSqlList({utils::getColumnName<Cols>()...});
        stmt % utils::asSqlList(vals, ",\n");
        return stmt.str();
    }

private:
    template <class Tuple, std::size_t... Idxs>
    static std::vector<std::string> tupleToStrings(const Tuple &tuple,
                                                   std::index_sequence<Idxs...>) {
        return {utils::sqlVal(std::get<Idxs>(tuple).value)...};
    }

    std::vector<std::string> vals;
};

template <class, class...>
class ReturningInsert;

template <class... Cols>
class Insert : public _Insert<Insert<Cols...>, Cols...> {
public:
    template <class RetColumn>
    auto returning(const RetColumn &retCol) const {
        static_assert(utils::same_table<RetColumn, Cols...>::value,
                      "returned column should be from the same table as inserted columns");
        return ReturningInsert<RetColumn, Cols...>{
            retCol, *static_cast<const _Insert<Insert<Cols...>, Cols...> *>(this)};
    }

    std::string str() const {
        return this->strPrefix();
    }
};

template <class RetCol, class... Cols>
class ReturningInsert : public _Insert<ReturningInsert<RetCol, Cols...>, Cols...> {
    static_assert(utils::same_table<RetCol, Cols...>::value,
                  "returned column should be from the same table as inserted columns");

public:
    using return_type = std::tuple<typename RetCol::value_type>;

    template <class _Insert2>
    ReturningInsert(const RetCol &, _Insert2 &&core)
        : _Insert<ReturningInsert<RetCol, Cols...>, Cols...>{std::forward<_Insert2>(core)} {
    }

    std::string str() const {
        boost::format stmt("%1%\nRETURNING %2%");
        stmt % this->strPrefix();
        stmt % utils::getColumnName<RetCol>();
        return stmt.str();
    }
};
}
}

#endif