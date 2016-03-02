#include <utils/fp_algorithm.h>

#include "QuestionsList.h"
#include "utils.h"

namespace server {
namespace io {

QuestionsList::QuestionType QuestionsList::QuestionTypeFromThrift(
    const communication::QuestionType::type &thrift) {
    switch (thrift) {
        case communication::QuestionType::type::SIMPLE:
            return QuestionsList::QuestionType::Simple;
        case communication::QuestionType::type::MULTIPLE_CHOICE:
            return QuestionsList::QuestionType::MultipleChoice;
        case communication::QuestionType::type::SORT:
            return QuestionsList::QuestionType::Sort;
    }
}

communication::QuestionType::type QuestionsList::QuestionTypeToThrift(
    const QuestionsList::QuestionType &type) {
    switch (type) {
        case QuestionsList::QuestionType::Simple:
            return communication::QuestionType::type::SIMPLE;
        case QuestionsList::QuestionType::MultipleChoice:
            return communication::QuestionType::type::MULTIPLE_CHOICE;
        case QuestionsList::QuestionType::Sort:
            return communication::QuestionType::type::SORT;
    }
}

communication::QuestionsList QuestionsList::toThrift() const {
    communication::QuestionsList thrift;
    ::utils::transform(questionsOrder, thrift.questionsOrder, QuestionTypeToThrift);
    thrift.simpleQuestions = ioToThrift(simpleQuestions);
    thrift.multipleChoiceQuestions = ioToThrift(multipleChoiceQuestions);
    thrift.sortQuestions = ioToThrift(sortQuestions);
    return thrift;
}
}
}