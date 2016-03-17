#include "QuestionType.h"

namespace server {
namespace io {

QuestionType questionTypeFromThrift(const communication::QuestionType::type &thrift) {
    switch (thrift) {
        case communication::QuestionType::type::SIMPLE:
            return QuestionType::Simple;
        case communication::QuestionType::type::MULTIPLE_CHOICE:
            return QuestionType::MultipleChoice;
        case communication::QuestionType::type::SORT:
            return QuestionType::Sort;
    }
}

communication::QuestionType::type questionTypeToThrift(const QuestionType &type) {
    switch (type) {
        case QuestionType::Simple:
            return communication::QuestionType::type::SIMPLE;
        case QuestionType::MultipleChoice:
            return communication::QuestionType::type::MULTIPLE_CHOICE;
        case QuestionType::Sort:
            return communication::QuestionType::type::SORT;
    }
}
}
}