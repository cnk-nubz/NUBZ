#include <server/io/utils.h>

#include "MultipleChoiceQuestion.h"

namespace server {
namespace io {
namespace output {

communication::MultipleChoiceQuestion MultipleChoiceQuestion::toThrift() const {
    auto res = communication::MultipleChoiceQuestion{};
    res.questionId = ID;
    res.name = name;
    res.question = question;
    res.singleAnswer = singleAnswer;
    res.options = ioToThrift(options);
    return res;
}

MultipleChoiceQuestion::MultipleChoiceQuestion(
    const repository::MultipleChoiceQuestions::Question &repo) {
    ID = repo.ID;
    name = repo.name;
    question = repo.question;
    singleAnswer = repo.singleAnswer;
    options = std::vector<Option>(repo.options.begin(), repo.options.end());
}

communication::MultipleChoiceQuestionOption MultipleChoiceQuestion::Option::toThrift() const {
    auto res = communication::MultipleChoiceQuestionOption{};
    res.optionId = ID;
    res.text = text;
    return res;
}

MultipleChoiceQuestion::Option::Option(
    const repository::MultipleChoiceQuestions::Question::Option &repo)
    : ID(repo.ID), text(repo.text) {
}
}
}
}