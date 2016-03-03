#include "MultipleChoiceQuestion.h"
#include "utils.h"

namespace server {
namespace io {

communication::MultipleChoiceQuestion MultipleChoiceQuestion::toThrift() const {
    communication::MultipleChoiceQuestion thrift;
    thrift.questionId = ID;
    thrift.name = name;
    thrift.question = question;
    thrift.singleAnswer = singleAnswer;
    thrift.options = ioToThrift(options);
    return thrift;
}

MultipleChoiceQuestion::MultipleChoiceQuestion(
    const repository::MultipleChoiceQuestions::Question &repo) {
    ID = repo.ID;
    name = repo.name;
    question = repo.question;
    singleAnswer = repo.singleAnswer;
    options = repoToIO<Option>(repo.options);
}

communication::MultipleChoiceQuestionOption MultipleChoiceQuestion::Option::toThrift() const {
    communication::MultipleChoiceQuestionOption thrift;
    thrift.optionId = ID;
    thrift.text = text;
    return thrift;
}

MultipleChoiceQuestion::Option::Option(
    const repository::MultipleChoiceQuestions::Question::Option &repo)
    : ID(repo.ID), text(repo.text) {
}
}
}