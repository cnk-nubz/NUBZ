#ifndef DB_CMD__GET_MULTIPLE_CHOICE_QUESTION_OPTIONS__H
#define DB_CMD__GET_MULTIPLE_CHOICE_QUESTION_OPTIONS__H

#include <vector>
#include <cstdint>

#include <boost/optional.hpp>

#include <db/struct/MultipleChoiceQuestionOption.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetMultipleChoiceQuestionOptions {
public:
    GetMultipleChoiceQuestionOptions() = default;
    GetMultipleChoiceQuestionOptions(std::int32_t questionId);

    const std::vector<MultipleChoiceQuestionOption> operator()(DatabaseSession &session);
    const std::vector<MultipleChoiceQuestionOption> &getResult() const;

    boost::optional<std::int32_t> questionId;
    boost::optional<std::int32_t> optionId;

private:
    std::vector<MultipleChoiceQuestionOption> result;

    std::string createQuery() const;
};
}
}

#endif