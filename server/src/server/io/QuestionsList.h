#ifndef SERVER_IO__QUESTIONS_LIST__H
#define SERVER_IO__QUESTIONS_LIST__H

#include <vector>

#include <communication/structs_types.h>

#include <repository/Experiments.h>

#include "output/MultipleChoiceQuestion.h"
#include "output/SimpleQuestion.h"
#include "output/SortQuestion.h"

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
    using repo_t = repository::Experiment::Survey;

    QuestionsList() = default;
    thrift_t toThrift() const;

    QuestionsList(const repo_t &repo);

    std::vector<QuestionType> questionsOrder;
    std::vector<output::SimpleQuestion> simpleQuestions;
    std::vector<output::MultipleChoiceQuestion> multipleChoiceQuestions;
    std::vector<output::SortQuestion> sortQuestions;
};
}
}

#endif