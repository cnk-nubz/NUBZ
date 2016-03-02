#ifndef DB_CMD__GET_MULTIPLE_CHOICE_QUESTION_OPTIONS__H
#define DB_CMD__GET_MULTIPLE_CHOICE_QUESTION_OPTIONS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/MultipleChoiceQuestionOption.h>

namespace db {
namespace cmd {

class GetMultipleChoiceQuestionOptions {
public:
    GetMultipleChoiceQuestionOptions() = default;
    GetMultipleChoiceQuestionOptions(std::int32_t questionId);

    std::vector<MultipleChoiceQuestionOption> operator()(DatabaseSession &session);
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