#include "SortQuestion.h"
#include "utils.h"

namespace server {
namespace io {

communication::SortQuestion SortQuestion::toThrift() const {
    communication::SortQuestion thrift;
    thrift.questionId = ID;
    thrift.name = name;
    thrift.question = question;
    thrift.options = ioToThrift(options);
    return thrift;
}

SortQuestion::SortQuestion(const repository::SortQuestions::Question &repo) {
    ID = repo.ID;
    name = repo.name;
    question = repo.question;
    options = repoToIO<Option>(repo.options);
}

communication::SortQuestionOption SortQuestion::Option::toThrift() const {
    communication::SortQuestionOption thrift;
    thrift.optionId = ID;
    thrift.text = text;
    return thrift;
}

SortQuestion::Option::Option(const repository::SortQuestions::Question::Option &repo)
    : ID(repo.ID), text(repo.text) {
}
}
}