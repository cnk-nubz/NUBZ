#ifndef DB_FACTORY__MAP_IMAGE_FACTORY__H
#define DB_FACTORY__MAP_IMAGE_FACTORY__H

#include <boost/lexical_cast.hpp>

#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct MapImageFactory {
            using Product = MapImage;

            static Product create(const std::vector<boost::optional<std::string>> &raw) {
                assert(raw.size() == fieldsOrder().size());

                MapImage res;
                res.filename = boost::lexical_cast<decltype(res.filename)>(raw[0].value());
                res.version = boost::lexical_cast<decltype(res.version)>(raw[1].value());
                res.level = boost::lexical_cast<decltype(res.level)>(raw[2].value());

                return res;
            }

            static const std::vector<std::string> &fieldsOrder() {
                using namespace db::info::map_images;
                static const std::vector<std::string> order = {colFilename, colVersion, colLevel};
                return order;
            }
        };
    }
}

#endif