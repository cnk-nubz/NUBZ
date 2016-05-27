#ifndef REPOSITORY__SIMPLE_QUESTIONS__H
#define REPOSITORY__SIMPLE_QUESTIONS__H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

/*
 * Validation:
 * Every method validates `ID`, `name`
 * and therefore can throw InvalidData or DuplicateName
 *
 * Deletion:
 * `remove` throws InUse if some experiment uses given question (reference count != 0)
 */
class SimpleQuestions {
public:
    struct Question {
        std::int32_t ID;
        std::string name;
        std::string question;
        bool numberAnswer;
    };

    SimpleQuestions(db::DatabaseSession &session);

    void incReferenceCount(std::int32_t ID);
    void decReferenceCount(std::int32_t ID);

    // non-throwing get
    boost::optional<Question> getOpt(std::int32_t ID);

    Question get(std::int32_t ID);
    std::vector<Question> getAll();

    void remove(std::int32_t ID);

    void insert(Question *simpleQuestion);
    void insert(std::vector<Question> *simpleQuestion);

private:
    void checkName(const std::string &name);

    db::DatabaseSession &session;
};

using SimpleQuestion = SimpleQuestions::Question;
}

#endif