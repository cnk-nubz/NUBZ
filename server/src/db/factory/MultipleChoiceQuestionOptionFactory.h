#ifndef DB_FACTORY__MULTIPLE_CHOICE_QUESTION_OPTION_FACTORY__H
#define DB_FACTORY__MULTIPLE_CHOICE_QUESTION_OPTION_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/struct/MultipleChoiceQuestionOption.h>
#include <db/db_info.h>

namespace db {
namespace factory {

struct MultipleChoiceQuestionOptionFactory {
    using Product = MultipleChoiceQuestionOption;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif