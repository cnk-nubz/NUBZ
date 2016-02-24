#ifndef DB_FACTORY__SORT_QUESTION_OPTION_FACTORY__H
#define DB_FACTORY__SORT_QUESTION_OPTION_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/struct/SortQuestionOption.h>
#include <db/db_info.h>

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