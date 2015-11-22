#ifndef DB_FACTORY__SINGLE_FIELD_FACTORY__H
#define DB_FACTORY__SINGLE_FIELD_FACTORY__H

#include <boost/lexical_cast.hpp>

namespace db {
    namespace factory {
        template <class T>
        struct SingleFieldFactory {
            using Product = T;

            static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept {
                Product res;
                assert(raw.size() == 1);
                assert(boost::conversion::try_lexical_convert(raw[0].value(), res));
                return res;
            }
        };
    }
}

#endif