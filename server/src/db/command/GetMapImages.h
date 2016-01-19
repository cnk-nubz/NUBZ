#ifndef DB_CMD__GET_MAP_IMAGES__H
#define DB_CMD__GET_MAP_IMAGES__H

#include <db/struct/MapImage.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetMapImages {
public:
    GetMapImages() = default;
    GetMapImages(std::int32_t floor);

    const std::vector<MapImage> &operator()(DatabaseSession &session);
    const std::vector<MapImage> &getResult() const;

    boost::optional<std::int32_t> minVersion;
    boost::optional<std::int32_t> floor;

private:
    std::vector<MapImage> result;

    std::string createQuery() const;
};
}
}

#endif