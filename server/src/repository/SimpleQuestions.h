#ifndef REPOSITORY__SIMPLE_QUESTIONS__H
#define REPOSITORY__SIMPLE_QUESTIONS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

class SimpleQuestions {
public:
    struct Question {
        std::int32_t ID;
        std::string name;
        std::string question;
        bool numberAnswer;
    };

    SimpleQuestions(db::DatabaseSession &session);

    boost::optional<Question> get(std::int32_t ID);
    std::vector<Question> getAll();

    void remove(std::int32_t ID);
    void removeAll();

    // ID will be saved in the given struct
    void insert(Question *simpleQuestion);
    void insert(std::vector<Question> *simpleQuestion);

private:
    db::DatabaseSession &session;
};

using SimpleQuestion = SimpleQuestions::Question;
}

#endif