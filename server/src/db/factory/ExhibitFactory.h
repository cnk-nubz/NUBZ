#ifndef DB_FACTORY__EXHIBIT_FACTORY__H
#define DB_FACTORY__EXHIBIT_FACTORY__H

#include <boost/lexical_cast.hpp>

#include "db/struct/Exhibit.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct ExhibitFactory {
            using Product = Exhibit;

            inline static Product create(
                const std::vector<boost::optional<std::string>> &raw) noexcept {
                Exhibit res;
                assert(raw.size() == fieldsOrder().size());
                assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
                assert(boost::conversion::try_lexical_convert(raw[1].value(), res.name));
                assert(boost::conversion::try_lexical_convert(raw[2].value(), res.version));
                res.frame = createFrame(raw);

                return res;
            }

            inline static boost::optional<MapElementFrame> createFrame(
                const std::vector<boost::optional<std::string>> &raw) noexcept {
                bool exist = raw[3].is_initialized();
                for (std::size_t i = 4; i < raw.size(); i++) {
                    assert(exist == raw[i].is_initialized());
                }
                if (!exist) {
                    return {};
                }

                MapElementFrame res;
                assert(boost::conversion::try_lexical_convert(raw[3].value(), res.x));
                assert(boost::conversion::try_lexical_convert(raw[4].value(), res.y));
                assert(boost::conversion::try_lexical_convert(raw[5].value(), res.width));
                assert(boost::conversion::try_lexical_convert(raw[6].value(), res.height));
                assert(boost::conversion::try_lexical_convert(raw[7].value(), res.mapLevel));
                return res;
            }

            inline static const std::vector<std::string> &fieldsOrder() noexcept {
                using namespace db::info::exhibits;
                static const std::vector<std::string> order = {
                    colId, colName, colVersion, colMapFrameX, colMapFrameY, colMapFrameWidth,
                    colMapFrameHeight, colMapLevel};
                return order;
            }
        };
    }
}

#endif