#ifndef DB_CMD__GET_SORT_QUESTION_OPTIONS__H
#define DB_CMD__GET_SORT_QUESTION_OPTIONS__H

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>
#include <db/struct/SortQuestionOption.h>

namespace db {
namespace cmd {

class GetSortQuestionOptions {
public:
    GetSortQuestionOptions() = default;
    GetSortQuestionOptions(std::int32_t questionId);

    std::vector<SortQuestionOption> operator()(DatabaseSession &session);
    const std::vector<SortQuestionOption> &getResult() const;

    boost::optional<std::int32_t> questionId;
    boost::optional<std::int32_t> optionId;

private:
    std::vector<SortQuestionOption> result;

    std::string createQuery() const;
};
}
}

#endif