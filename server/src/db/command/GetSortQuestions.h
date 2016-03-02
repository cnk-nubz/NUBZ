#ifndef DB_CMD__GET_SORT_QUESTIONS__H
#define DB_CMD__GET_SORT_QUESTIONS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/SortQuestion.h>

namespace db {
namespace cmd {

class GetSortQuestions {
public:
    GetSortQuestions() = default;
    GetSortQuestions(std::int32_t questionId);

    std::vector<SortQuestion> operator()(DatabaseSession &session);
    const std::vector<SortQuestion> &getResult() const;

private:
    boost::optional<std::int32_t> questionId;
    std::vector<SortQuestion> result;

    std::string createQuery() const;
};
}
}

#endif