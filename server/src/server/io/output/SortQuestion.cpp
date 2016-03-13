#include <server/io/utils.h>

#include "SortQuestion.h"

namespace server {
namespace io {
namespace output {

communication::SortQuestion SortQuestion::toThrift() const {
    auto res = communication::SortQuestion{};
    res.questionId = ID;
    res.name = name;
    res.question = question;
    res.options = ioToThrift(options);
    return res;
}

SortQuestion::SortQuestion(const repository::SortQuestions::Question &repo) {
    ID = repo.ID;
    name = repo.name;
    question = repo.question;
    options = std::vector<Option>(repo.options.begin(), repo.options.end());
}

communication::SortQuestionOption SortQuestion::Option::toThrift() const {
    auto res = communication::SortQuestionOption{};
    res.optionId = ID;
    res.text = text;
    return res;
}

SortQuestion::Option::Option(const repository::SortQuestions::Question::Option &repo)
    : ID(repo.ID), text(repo.text) {
}
}
}
}