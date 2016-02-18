#ifndef DB_CMD__GET_SIMPLE_QUESTIONS__H
#define DB_CMD__GET_SIMPLE_QUESTIONS__H

#include <boost/optional.hpp>

#include <db/struct/SimpleQuestion.h>
#include <db/DatabaseSession.h>

namespace db {
namespace cmd {

class GetSimpleQuestions {
public:
    GetSimpleQuestions() = default;
    GetSimpleQuestions(std::int32_t simpleQId);

    const std::vector<SimpleQuestion> operator()(DatabaseSession &session);
    const std::vector<SimpleQuestion> &getResult() const;

    boost::optional<std::int32_t> simpleQuestionId;

private:
    std::vector<SimpleQuestion> result;

    std::string createQuery() const;
};
}
}

#endif