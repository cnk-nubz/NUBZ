#ifndef DB_FACTORY__SORT_QUESTION_FACTORY__H
#define DB_FACTORY__SORT_QUESTION_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/struct/SortQuestion.h>
#include <db/db_info.h>

namespace db {
namespace factory {

struct SortQuestionFactory {
    using Product = SortQuestion;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif