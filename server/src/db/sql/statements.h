#ifndef DB_SQL__STATEMENTS__H
#define DB_SQL__STATEMENTS__H

#include <array>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include "elements.h"

namespace db {
namespace sql {

class SqlSelect;
class SqlDelete;
class SqlUpdate;

template <unsigned numOfParams, bool returningAvailable>
class SqlInsert;

template <class T>
class SqlWhereFunction {
public:
    SqlWhereFunction() = default;
    ~SqlWhereFunction() = default;

    SqlWhereFunction(const SqlWhereFunction &);
    SqlWhereFunction(SqlWhereFunction &&) = default;
    SqlWhereFunction &operator=(const SqlWhereFunction &);
    SqlWhereFunction &operator=(SqlWhereFunction &&) = default;

    T &where(const Cond &cond);

protected:
    std::string whereStmt() const;

private:
    std::unique_ptr<Cond> whereCond;
};

class SqlSelectBegin;
class SqlInsertBegin;

class Sql {
public:
    Sql() = delete;
    static SqlSelectBegin select(const std::vector<std::string> &columns);
    static SqlInsertBegin insertInto(const std::string &into);
    static SqlUpdate update(const std::string &what);
    static SqlDelete deleteFrom(const std::string &from);
};

class SqlSelectBegin {
    friend class Sql;

public:
    SqlSelect from(const std::string &from) const;

private:
    const std::vector<std::string> columns;
    SqlSelectBegin(const std::vector<std::string> &columns);
};

class SqlSelect : public SqlWhereFunction<SqlSelect> {
    friend class SqlSelectBegin;

public:
    operator std::string() const;
    std::string str() const;

private:
    const std::vector<std::string> columns;
    const std::string from;
    SqlSelect(const std::vector<std::string> &columns, const std::string &from);
};

class SqlInsertBegin {
    friend class Sql;

public:
    template <class... Ts>
    auto what(Ts &&... ts) const -> SqlInsert<sizeof...(Ts), true>;

private:
    const std::string &into;
    SqlInsertBegin(const std::string &into);
};

template <unsigned numOfParams, bool returningAvailable>
class SqlInsert {
    friend class SqlInsert<numOfParams, true>;
    friend class SqlInsertBegin;

public:
    template <class... Ts>
    SqlInsert<numOfParams, returningAvailable> &values(Ts &&... ts);

    template <bool Hack = true, class = std::enable_if_t<returningAvailable && Hack>>
    SqlInsert<numOfParams, false> returning(const std::string &column) const;

    operator std::string() const;
    std::string str() const;

private:
    std::array<std::string, numOfParams> what;
    const std::string into;
    std::vector<std::array<std::string, numOfParams>> vals;
    boost::optional<std::string> inReturn;

    SqlInsert(const std::array<std::string, numOfParams> &what, const std::string &into);
    std::string returningStmt() const;
};

class SqlUpdate : public SqlWhereFunction<SqlUpdate> {
    friend class Sql;

public:
    template <class T>
    SqlUpdate &set(const std::string &colName, const T &newValue);

    operator std::string() const;
    std::string str() const;

private:
    const std::string what;
    std::vector<std::string> sets;
    SqlUpdate(const std::string &what);
};

class SqlDelete : public SqlWhereFunction<SqlDelete> {
    friend class Sql;

public:
    operator std::string() const;
    std::string str() const;

private:
    const std::string from;
    SqlDelete(const std::string &from);
};
}
}

namespace db {
namespace sql {

template <class Container>
std::string asSqlList(const Container &list, const std::string &separator = ", ") {
    std::string res;
    auto it = list.begin();
    while (it != list.end()) {
        res += *it;
        if (++it != list.end()) {
            res += separator;
        }
    }
    return res;
}

// Select
inline SqlSelectBegin Sql::select(const std::vector<std::string> &columns) {
    return SqlSelectBegin(columns);
}

inline SqlSelectBegin::SqlSelectBegin(const std::vector<std::string> &columns) : columns(columns) {
}

inline SqlSelect SqlSelectBegin::from(const std::string &from) const {
    return SqlSelect(columns, from);
}

inline SqlSelect::SqlSelect(const std::vector<std::string> &columns, const std::string &from)
    : columns(columns), from(from) {
}

inline SqlSelect::operator std::string() const {
    return str();
}

inline std::string SqlSelect::str() const {
    boost::format res("SELECT %1% \nFROM %2% %3%");
    res % asSqlList(columns);
    res % from;
    res % whereStmt();
    return res.str();
}

// Update
inline SqlUpdate Sql::update(const std::string &what) {
    return SqlUpdate(what);
}

inline SqlUpdate::SqlUpdate(const std::string &what) : what(what) {
}

template <class T>
SqlUpdate &SqlUpdate::set(const std::string &colName, const T &newValue) {
    boost::format set("%1% = %2%");
    set % colName;
    set % SqlString(newValue).str();
    sets.push_back(set.str());
    return *this;
}

inline SqlUpdate::operator std::string() const {
    return str();
}

inline std::string SqlUpdate::str() const {
    assert(sets.size() > 0);

    boost::format res("UPDATE %1% \nSET \n%2% %3%");
    res % what;
    res % asSqlList(sets, ",\n");
    res % whereStmt();
    return res.str();
}

// Delete
inline SqlDelete Sql::deleteFrom(const std::string &from) {
    return SqlDelete(from);
}

inline SqlDelete::SqlDelete(const std::string &from) : from(from) {
}

inline SqlDelete::operator std::string() const {
    return str();
}

inline std::string SqlDelete::str() const {
    boost::format res("DELETE FROM %1% %2%");
    res % from;
    res % whereStmt();
    return res.str();
}

// Insert
inline SqlInsertBegin Sql::insertInto(const std::string &into) {
    return SqlInsertBegin(into);
}

inline SqlInsertBegin::SqlInsertBegin(const std::string &into) : into(into) {
}

template <class... Ts>
auto SqlInsertBegin::what(Ts &&... ts) const -> SqlInsert<sizeof...(Ts), true> {
    std::array<std::string, sizeof...(ts)> args{{std::forward<Ts>(ts)...}};
    return SqlInsert<sizeof...(ts), true>(args, into);
}

template <unsigned numOfParams, bool returningAvailable>
SqlInsert<numOfParams, returningAvailable>::SqlInsert(
    const std::array<std::string, numOfParams> &what, const std::string &into)
    : what(what), into(into), vals() {
}

template <unsigned numOfParams, bool returningAvailable>
template <class... Ts>
SqlInsert<numOfParams, returningAvailable> &SqlInsert<numOfParams, returningAvailable>::values(
    Ts &&... ts) {
    static_assert(sizeof...(ts) == numOfParams, "incorrect number of arguments");
    std::array<SqlString, numOfParams> sqlStrings{{std::forward<Ts>(ts)...}};
    vals.emplace_back();
    std::copy(sqlStrings.begin(), sqlStrings.end(), vals.back().begin());
    return *this;
}

template <unsigned numOfParams, bool returningAvailable>
template <bool Hack, class>
SqlInsert<numOfParams, false> SqlInsert<numOfParams, returningAvailable>::returning(
    const std::string &column) const {
    auto insert = SqlInsert<numOfParams, false>(what, into);
    insert.vals = vals;
    insert.inReturn = column;
    return insert;
}

template <unsigned numOfParams, bool returningAvailable>
SqlInsert<numOfParams, returningAvailable>::operator std::string() const {
    return str();
}

template <unsigned numOfParams, bool returningAvailable>
std::string SqlInsert<numOfParams, returningAvailable>::str() const {
    assert(vals.size() > 0);

    boost::format res("INSERT INTO %1% \n(%2%) \nVALUES \n%3% \n%4%");
    res % into;
    res % asSqlList(what);

    std::vector<std::string> rows;
    for (const auto &row : vals) {
        rows.push_back("(" + asSqlList(row) + ")");
    }
    res % asSqlList(rows, ",\n");
    res % returningStmt();

    return res.str();
}

template <unsigned numOfParams, bool returningAvailable>
std::string SqlInsert<numOfParams, returningAvailable>::returningStmt() const {
    return (inReturn) ? "RETURNING " + inReturn.value() : "";
}

// Where
template <class T>
SqlWhereFunction<T>::SqlWhereFunction(const SqlWhereFunction<T> &that) {
    if (that.whereCond && this != &that) {
        whereCond.reset(new Cond(*that.whereCond));
    }
}

template <class T>
SqlWhereFunction<T> &SqlWhereFunction<T>::operator=(const SqlWhereFunction<T> &that) {
    if (that.whereCond && this != &that) {
        whereCond.reset(new Cond(*that.whereCond));
    }
}

template <class T>
T &SqlWhereFunction<T>::where(const Cond &cond) {
    if (!whereCond) {
        whereCond.reset(new Cond(cond));
    } else {
        whereCond.reset(new Cond(*whereCond && cond));
    }
    return *static_cast<T *>(this);
}

template <class T>
std::string SqlWhereFunction<T>::whereStmt() const {
    if (whereCond) {
        return "\nWHERE " + whereCond->str();
    } else {
        return {};
    }
}
}
}

#endif