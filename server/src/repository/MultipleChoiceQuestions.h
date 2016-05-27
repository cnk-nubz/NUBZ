#ifndef REPOSITORY__MULTIPLE_CHOICE_QUESTIONS__H
#define REPOSITORY__MULTIPLE_CHOICE_QUESTIONS__H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

/*
 * Validation:
 * Every method validates `ID`, `name`, options (amount & duplicates)
 * and therefore can throw InvalidData or DuplicateName
 *
 * Deletion:
 * `remove` throws InUse if some experiment uses given question (reference count != 0)
 */
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

    void incReferenceCount(std::int32_t ID);
    void decReferenceCount(std::int32_t ID);

    // non-throwing get
    boost::optional<Question> getOpt(std::int32_t ID);

    Question get(std::int32_t ID);
    std::vector<Question> getAll();

    void remove(std::int32_t ID);

    void insert(Question *multipleChoiceQuestion);
    void insert(std::vector<Question> *multipleChoiceQuestion);

private:
    std::vector<Question::Option> getOptions(std::int32_t questionID);

    void checkName(const std::string &name);
    void checkOptions(const std::vector<Question::Option> &options);

    // ID will be saved in the given struct
    void insert(Question::Option *questionOption, std::int32_t questionID);

    db::DatabaseSession &session;
};

using MultipleChoiceQuestion = MultipleChoiceQuestions::Question;
}

#endif