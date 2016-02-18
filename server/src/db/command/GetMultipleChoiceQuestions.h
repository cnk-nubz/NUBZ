#ifndef DB_CMD__GET_MULTIPLE_CHOICE_QUESTIONS__H
#define DB_CMD__GET_MULTIPLE_CHOICE_QUESTIONS__H

#include <vector>
#include <cstdint>

#include <boost/optional.hpp>

#include <db/struct/MultipleChoiceQuestion.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetMultipleChoiceQuestions {
public:
    GetMultipleChoiceQuestions() = default;
    GetMultipleChoiceQuestions(std::int32_t questionId);

    const std::vector<MultipleChoiceQuestion> operator()(DatabaseSession &session);
    const std::vector<MultipleChoiceQuestion> &getResult() const;

private:
    boost::optional<std::int32_t> questionId;
    std::vector<MultipleChoiceQuestion> result;

    std::string createQuery() const;
};
}
}

#endif