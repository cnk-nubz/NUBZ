#ifndef DB_FACTORY__SINGLE_FIELD_FACTORY__H
#define DB_FACTORY__SINGLE_FIELD_FACTORY__H

#include <boost/lexical_cast.hpp>

namespace db {
    namespace factory {
        template <class T>
        struct SingleFieldFactory {
            using Product = T;

            static Product create(const std::vector<boost::optional<std::string>> &raw) {
                assert(raw.size() == 1);
                return boost::lexical_cast<T>(raw[0].value());
            }
        };
    }
}

#endif