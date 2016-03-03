#ifndef REPOSITORY__MULTIPLE_CHOICE_QUESTIONS__H
#define REPOSITORY__MULTIPLE_CHOICE_QUESTIONS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

class MultipleChoiceQuestions {
public:
    struct Question {
        struct Option {
            std::int32_t ID;
            std::string text;
        };

        std::int32_t ID;
        std::string name;
        std::string question;
        bool singleAnswer;
        std::vector<Option> options;
    };

    MultipleChoiceQuestions(db::DatabaseSession &session);

    boost::optional<Question> get(std::int32_t ID);
    std::vector<Question> getAll();

    void remove(std::int32_t ID);
    void removeAll();

    // ID will be saved in the given struct
    void insert(Question *MultipleChoiceQuestion);
    void insert(std::vector<Question> *MultipleChoiceQuestion);

private:
    std::vector<Question::Option> getOptions(std::int32_t questionID);

    // ID will be saved in the given struct
    void insert(Question::Option *questionOption, std::int32_t questionID);

    db::DatabaseSession &session;
};

using MultipleChoiceQuestion = MultipleChoiceQuestions::Question;
}

#endif