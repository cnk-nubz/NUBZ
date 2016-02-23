#include <utils/fp_algorithm.h>

#include "utils.h"
#include "QuestionsList.h"

namespace server {
namespace io {

QuestionsList::QuestionType QuestionsList::QuestionTypeFromThrift(
    const communication::QuestionType::type &thrift) {
    switch (thrift) {
        case communication::QuestionType::type::SIMPLE:
            return QuestionsList::QuestionType::Simple;
        case communication::QuestionType::type::MULTIPLE_CHOICE:
            return QuestionsList::QuestionType::MultipleChoice;
    }
}

communication::QuestionType::type QuestionsList::QuestionTypeToThrift(
    const QuestionsList::QuestionType &type) {
    switch (type) {
        case QuestionsList::QuestionType::Simple:
            return communication::QuestionType::type::SIMPLE;
        case QuestionsList::QuestionType::MultipleChoice:
            return communication::QuestionType::type::MULTIPLE_CHOICE;
    }
}

communication::QuestionsList QuestionsList::toThrift() const {
    communication::QuestionsList thrift;
    ::utils::transform(questionsOrder, thrift.questionsOrder, QuestionTypeToThrift);
    thrift.simpleQuestions = ioToThrift(simpleQuestions);
    thrift.multipleChoiceQuestions = ioToThrift(multipleChoiceQuestions);
    return thrift;
}
}
}