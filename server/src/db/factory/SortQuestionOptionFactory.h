#ifndef DB_FACTORY__SORT_QUESTION_OPTION_FACTORY__H
#define DB_FACTORY__SORT_QUESTION_OPTION_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/db_info.h>
#include <db/struct/SortQuestionOption.h>

namespace db {
namespace factory {

struct SortQuestionOptionFactory {
    using Product = SortQuestionOption;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif