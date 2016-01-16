#ifndef DB_SQL__ELEMENTS__H
#define DB_SQL__ELEMENTS__H

#include <string>
#include <vector>
#include <type_traits>

#include <boost/format.hpp>

namespace db {
    namespace sql {
        class E;
        class Cond;
        struct Null;
        class SqlString;
    }
}

namespace db {
    namespace sql {
        class C {
        public:
            C(const std::string &colName);

            template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
            Cond operator<(const T &arg) const;

            template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
            Cond operator<=(const T &arg) const;

            template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
            Cond operator>(const T &arg) const;

            template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
            Cond operator>=(const T &arg) const;

            template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
            Cond operator==(const T &arg) const;

            template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
            Cond operator!=(const T &arg) const;

            Cond operator==(const std::string &arg) const;
            Cond operator!=(const std::string &arg) const;

            Cond operator==(const char *arg) const;
            Cond operator!=(const char *arg) const;

            Cond operator==(const Null &) const;
            Cond operator!=(const Null &) const;

        private:
            const std::string colName;
        };

        class Cond {
            friend class C;

        public:
            Cond operator&&(const Cond &cond);
            Cond operator||(const Cond &cond);

            operator std::string() const;
            std::string str() const;

        private:
            const std::string lhs;
            const std::string op;
            const std::string rhs;

            Cond(const std::string &a, const std::string &op, const std::string &b);
        };

        struct Null {
            operator std::string() const;
            std::string str() const;
        };

        class SqlString {
        public:
            SqlString(const std::string &raw);
            SqlString(const char *raw);
            SqlString(bool raw);
            SqlString(const Null &null);

            template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
            SqlString(const T &t);

            operator std::string() const;
            std::string str() const;

        private:
            std::string safe;

            void addQuote();
        };
    }
}

namespace db {
    namespace sql {
        template <class T, class>
        Cond C::operator<(const T &arg) const {
            return Cond(colName, "<", SqlString(arg));
        }

        template <class T, class>
        Cond C::operator<=(const T &arg) const {
            return Cond(colName, "<=", SqlString(arg));
        }

        template <class T, class>
        Cond C::operator>(const T &arg) const {
            return Cond(colName, ">", SqlString(arg));
        }

        template <class T, class>
        Cond C::operator>=(const T &arg) const {
            return Cond(colName, ">=", SqlString(arg));
        }

        template <class T, class>
        Cond C::operator==(const T &arg) const {
            return Cond(colName, "=", SqlString(arg));
        }

        template <class T, class>
        Cond C::operator!=(const T &arg) const {
            return Cond(colName, "<>", SqlString(arg));
        }

        inline C::C(const std::string &colName) : colName(colName) {
        }

        inline Cond C::operator==(const char *arg) const {
            return *this == std::string(arg);
        }

        inline Cond C::operator!=(const char *arg) const {
            return *this != std::string(arg);
        }

        inline Cond C::operator==(const std::string &arg) const {
            return Cond(colName, "=", SqlString(arg));
        }

        inline Cond C::operator!=(const std::string &arg) const {
            return Cond(colName, "<>", SqlString(arg));
        }

        inline Cond C::operator==(const Null &arg) const {
            return Cond(colName, "IS", arg.str());
        }

        inline Cond C::operator!=(const Null &arg) const {
            return Cond(colName, "IS NOT", arg.str());
        }

        inline Cond::Cond(const std::string &a, const std::string &op, const std::string &b)
            : lhs(a), op(op), rhs(b) {
        }

        inline Cond Cond::operator&&(const Cond &cond) {
            return Cond("(" + str() + ")", "AND", "(" + cond.str() + ")");
        }

        inline Cond Cond::operator||(const Cond &cond) {
            return Cond("(" + str() + ")", "OR", "(" + cond.str() + ")");
        }

        inline Cond::operator std::string() const {
            return str();
        }

        inline std::string Cond::str() const {
            boost::format res("%1% %2% %3%");
            res % lhs % op % rhs;
            return res.str();
        }

        inline Null::operator std::string() const {
            return str();
        }

        inline std::string Null::str() const {
            return "NULL";
        }

        inline SqlString::SqlString(const char *raw) : SqlString(std::string(raw)) {
        }

        inline SqlString::SqlString(const std::string &raw) : safe(raw) {
            addQuote();
        }

        inline void SqlString::addQuote() {
            safe = "\'" + safe + "\'";
        }

        inline SqlString::SqlString(const Null &null) : safe(null.str()) {
        }

        inline SqlString::SqlString(bool raw) : safe(raw ? "true" : "false") {
        }

        template <class T, class>
        SqlString::SqlString(const T &t)
            : safe(std::to_string(t)) {
        }

        inline SqlString::operator std::string() const {
            return str();
        }

        inline std::string SqlString::str() const {
            return safe;
        }
    }
}

#endif