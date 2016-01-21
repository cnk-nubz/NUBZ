#ifndef DB_CMD__GET_EXPERIMENTS__H
#define DB_CMD__GET_EXPERIMENTS__H

#include <boost/optional.hpp>

#include <db/struct/Experiment.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetExperiments {
public:
    GetExperiments() = default;
    GetExperiments(std::int32_t experimentId);

    const std::vector<Experiment> &operator()(DatabaseSession &session);
    const std::vector<Experiment> &getResult() const;

    boost::optional<std::int32_t> experimentId;

private:
    std::vector<Experiment> result;

    std::string createQuery() const;
};
}
}

#endif