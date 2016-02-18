#ifndef DB_FACTORY__SINGLE_FIELD_FACTORY__H
#define DB_FACTORY__SINGLE_FIELD_FACTORY__H

#include <vector>
#include <unordered_set>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace db {
namespace factory {

template <class T>
struct SingleFieldFactory {
    using Product = T;

    inline static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept {
        assert(raw.size() == 1);
        assert(raw.front());

        Product res;
        assert(boost::conversion::try_lexical_convert(raw[0].value(), res));
        return res;
    }
};

template <>
struct SingleFieldFactory<bool> {
    using Product = bool;

    inline static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept {
        static const std::unordered_set<std::string> trueLiteral = {
            "TRUE", "t", "true", "y", "yes", "on", "1"};
        static const std::unordered_set<std::string> falseLiteral = {
            "FALSE", "f", "false", "n", "no", "off", "0"};

        assert(raw.size() == 1);
        assert(raw.front());

        if (trueLiteral.count(raw.front().value())) {
            return true;
        }
        if (falseLiteral.count(raw.front().value())) {
            return false;
        }

        assert(false);
    }
};
}
}

#endif