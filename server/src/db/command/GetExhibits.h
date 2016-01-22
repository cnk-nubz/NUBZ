#ifndef DB_CMD__GET_EXHIBITS__H
#define DB_CMD__GET_EXHIBITS__H

#include <boost/optional.hpp>

#include <db/struct/Exhibit.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetExhibits {
public:
    GetExhibits() = default;
    GetExhibits(std::int32_t exhibitId);

    const std::vector<Exhibit> operator()(DatabaseSession &session);
    const std::vector<Exhibit> &getResult() const;

    boost::optional<std::int32_t> exhibitId;
    boost::optional<std::int32_t> minVersion;

private:
    std::vector<Exhibit> result;

    std::string createQuery() const;
};
}
}

#endif