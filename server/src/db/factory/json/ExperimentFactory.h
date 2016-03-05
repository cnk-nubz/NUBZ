#ifndef DB_FACTORY_JSON__EXPERIMENT_FACTORY__H
#define DB_FACTORY_JSON__EXPERIMENT_FACTORY__H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <db/struct/Experiment.h>

namespace db {
namespace factory {
namespace json {

struct ExperimentFactory {
    using Product = Experiment;

    static std::string createJson(const Product &experiment);

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;
    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}
}

#endif