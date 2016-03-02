#ifndef DB_CMD__GET_MULTIPLE_CHOICE_QUESTIONS__H
#define DB_CMD__GET_MULTIPLE_CHOICE_QUESTIONS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/MultipleChoiceQuestion.h>

namespace db {
namespace cmd {

class GetMultipleChoiceQuestions {
public:
    GetMultipleChoiceQuestions() = default;
    GetMultipleChoiceQuestions(std::int32_t questionId);

    std::vector<MultipleChoiceQuestion> operator()(DatabaseSession &session);
    const std::vector<MultipleChoiceQuestion> &getResult() const;

private:
    boost::optional<std::int32_t> questionId;
    std::vector<MultipleChoiceQuestion> result;

    std::string createQuery() const;
};
}
}

#endif