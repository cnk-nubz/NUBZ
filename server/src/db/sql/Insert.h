#ifndef DB_SQL__INSERT__H
#define DB_SQL__INSERT__H

#include <string>
#include <cassert>

#include <boost/format.hpp>

#include "utils.h"

namespace db {
namespace sql {

template <class Subclass, class... Cols>
class InsertCore {
    template <class Subclass2, class... Cols2>
    friend class InsertCore;

    static_assert(utils::same_table<Cols...>::value, "all columns should be from the same table");

public:
    template <class Subclass2, class... Cols2>
    InsertCore(InsertCore<Subclass2, Cols2...> that)
        : vals(std::move(that.vals)) {
        static_assert(std::is_same<std::tuple<Cols...>, std::tuple<Cols2...>>::value,
                      "two InsertCores should have the same set of columns");
    }

    InsertCore() = default;

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
class Insert : public InsertCore<Insert<Cols...>, Cols...> {
public:
    template <class RetColumn>
    auto returning(const RetColumn &retCol) const {
        static_assert(utils::same_table<RetColumn, Cols...>::value,
                      "returned column should be from the same table as inserted columns");
        return ReturningInsert<RetColumn, Cols...>{
            retCol, *static_cast<const InsertCore<Insert<Cols...>, Cols...> *>(this)};
    }

    std::string str() const {
        return this->strPrefix();
    }
};

template <class RetCol, class... Cols>
class ReturningInsert : public InsertCore<ReturningInsert<RetCol, Cols...>, Cols...> {
    static_assert(utils::same_table<RetCol, Cols...>::value,
                  "returned column should be from the same table as inserted columns");

public:
    using return_type = std::tuple<typename RetCol::value_type>;

    template <class InsertCore2>
    ReturningInsert(const RetCol &, InsertCore2 &&core)
        : InsertCore<ReturningInsert<RetCol, Cols...>, Cols...>{std::forward<InsertCore2>(core)} {
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