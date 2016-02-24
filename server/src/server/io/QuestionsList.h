#ifndef SERVER_IO__QUESTIONS_LIST__H
#define SERVER_IO__QUESTIONS_LIST__H

#include <vector>

#include <communication/structs_types.h>

#include "SimpleQuestion.h"
#include "MultipleChoiceQuestion.h"
#include "SortQuestion.h"

namespace server {
namespace io {

struct QuestionsList {
    enum class QuestionType {
        Simple,
        MultipleChoice,
        Sort,
    };
    static QuestionType QuestionTypeFromThrift(const communication::QuestionType::type &thrift);
    static communication::QuestionType::type QuestionTypeToThrift(const QuestionType &type);

    using thrift_t = communication::QuestionsList;

    QuestionsList() = default;
    thrift_t toThrift() const;

    std::vector<QuestionType> questionsOrder;
    std::vector<SimpleQuestion> simpleQuestions;
    std::vector<MultipleChoiceQuestion> multipleChoiceQuestions;
    std::vector<SortQuestion> sortQuestions;
};
}
}

#endif