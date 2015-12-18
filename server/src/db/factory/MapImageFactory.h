#ifndef DB_FACTORY__MAP_IMAGE_FACTORY__H
#define DB_FACTORY__MAP_IMAGE_FACTORY__H

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct MapImageFactory {
            using Product = MapImage;

            static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

            static const std::vector<std::string> &fieldsOrder() noexcept;
        };
    }
}

#endif