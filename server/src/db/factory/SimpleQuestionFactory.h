#ifndef DB_FACTORY__SIMPLE_QUESTION_FACTORY__H
#define DB_FACTORY__SIMPLE_QUESTION_FACTORY__H

#include <vector>

#include <boost/optional.hpp>

#include <db/struct/SimpleQuestion.h>
#include <db/db_info.h>

namespace db {
namespace factory {

struct SimpleQuestionFactory {
    using Product = SimpleQuestion;

    static Product create(const std::vector<boost::optional<std::string>> &raw) noexcept;

    static const std::vector<std::string> &fieldsOrder() noexcept;
};
}
}

#endif