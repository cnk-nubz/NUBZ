#ifndef DB_FACTORY__MAP_IMAGE_FACTORY__H
#define DB_FACTORY__MAP_IMAGE_FACTORY__H

#include <boost/lexical_cast.hpp>

#include "db/struct/MapImage.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct MapImageFactory {
            using Product = MapImage;

            inline static Product create(
                const std::vector<boost::optional<std::string>> &raw) noexcept {
                MapImage res;
                assert(raw.size() == fieldsOrder().size());
                assert(boost::conversion::try_lexical_convert(raw[0].value(), res.filename));
                assert(boost::conversion::try_lexical_convert(raw[1].value(), res.version));
                assert(boost::conversion::try_lexical_convert(raw[2].value(), res.level));
                return res;
            }

            inline static const std::vector<std::string> &fieldsOrder() noexcept {
                using namespace db::info::map_images;
                static const std::vector<std::string> order = {colFilename, colVersion, colLevel};
                return order;
            }
        };
    }
}

#endif