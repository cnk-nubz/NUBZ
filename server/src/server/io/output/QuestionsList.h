#ifndef SERVER_IO_OUTPUT__QUESTIONS_LIST__H
#define SERVER_IO_OUTPUT__QUESTIONS_LIST__H

#include <vector>

#include <communication/structs_types.h>

#include <repository/Experiments.h>

#include <server/io/QuestionType.h>

#include "MultipleChoiceQuestion.h"
#include "SimpleQuestion.h"
#include "SortQuestion.h"

namespace server {
namespace io {
namespace output {

struct QuestionsList {
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
}

#endif