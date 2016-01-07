#ifndef DB_FACTORY__ACTION_FACTORY__H
#define DB_FACTORY__ACTION_FACTORY__H

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "db/struct/Action.h"
#include "db/db_info.h"

namespace db {
    namespace factory {
        struct ActionFactory {
            using Product = Action;

            static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

            static const std::vector<std::string> &fieldsOrder() noexcept;
        };
    }
}

#endif