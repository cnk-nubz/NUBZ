#ifndef DB_FACTORY__SINGLE_FIELD_FACTORY__H
#define DB_FACTORY__SINGLE_FIELD_FACTORY__H

#include <vector>

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
        assert(raw.size() == 1);
        assert(raw.front());

        if (raw.front().value() == "t") {
            return true;
        }
        if (raw.front().value() == "f") {
            return false;
        }
        
        assert(false);
    }
};
}
}

#endif