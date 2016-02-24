#ifndef SERVER_IO__SORT_QUESTION__H
#define SERVER_IO__SORT_QUESTION__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include "Question.h"

namespace server {
namespace io {

struct SortQuestion : public Question {
    struct Option {
        using thrift_t = communication::SortQuestionOption;

        Option() = default;
        thrift_t toThrift() const;

        std::int32_t ID;
        std::string text;
    };

    using thrift_t = communication::SortQuestion;

    SortQuestion() = default;
    thrift_t toThrift() const;

    std::vector<Option> options;
};
}
}

#endif