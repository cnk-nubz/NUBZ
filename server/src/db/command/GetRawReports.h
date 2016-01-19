#ifndef DB_CMD__GET_RAW_REPORTS__H
#define DB_CMD__GET_RAW_REPORTS__H

#include <boost/optional.hpp>

#include <db/struct/RawReport.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetRawReports {
public:
    GetRawReports() = default;
    GetRawReports(std::int32_t reportId);

    const std::vector<RawReport> &operator()(DatabaseSession &session);
    const std::vector<RawReport> &getResult() const;

private:
    const boost::optional<std::int32_t> reportId;
    std::vector<RawReport> result;

    std::string createQuery() const;
};
}
}

#endif