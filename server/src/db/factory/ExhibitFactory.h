#ifndef DB_FACTORY__EXHIBIT_FACTORY__H
#define DB_FACTORY__EXHIBIT_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/struct/Exhibit.h>
#include <db/db_info.h>

namespace db {
namespace factory {

struct ExhibitFactory {
    using Product = Exhibit;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;
    static boost::optional<MapElementFrame> createFrame(
        const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif