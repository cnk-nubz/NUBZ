#ifndef SERVER_IO__SORT_QUESTION__H
#define SERVER_IO__SORT_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include <repository/SortQuestions.h>

#include "Question.h"

namespace server {
namespace io {

struct SortQuestion : public Question {
    struct Option {
        using thrift_t = communication::SortQuestionOption;
        using repo_t = repository::SortQuestion::Option;

        Option() = default;
        thrift_t toThrift() const;

        Option(const repo_t &repo);

        std::int32_t ID;
        std::string text;
    };

    using thrift_t = communication::SortQuestion;
    using repo_t = repository::SortQuestion;

    SortQuestion() = default;
    thrift_t toThrift() const;

    SortQuestion(const repo_t &repo);

    std::vector<Option> options;
};
}
}

#endif