#ifndef DB_CMD__GET_CURRENT_EXPERIMENT__H
#define DB_CMD__GET_CURRENT_EXPERIMENT__H

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/Experiment.h>

namespace db {
namespace cmd {

class GetCurrentExperiment {
public:
    boost::optional<Experiment> operator()(DatabaseSession &session);
    const boost::optional<Experiment> &getResult() const;

private:
    boost::optional<Experiment> result;

    std::string createQuery() const;
};
}
}

#endif