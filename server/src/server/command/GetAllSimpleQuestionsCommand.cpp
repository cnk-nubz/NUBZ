#include <utils/fp_algorithm.h>

#include <db/command/GetSimpleQuestions.h>

#include <server/utils/io_translation.h>

#include "GetAllSimpleQuestionsCommand.h"

namespace server {
namespace command {

GetAllSimpleQuestionsCommand::GetAllSimpleQuestionsCommand(db::Database &db) : db(db) {
}

std::vector<io::SimpleQuestion> GetAllSimpleQuestionsCommand::operator()() {
    db::cmd::GetSimpleQuestions getSimpleQuestions;
    db.execute(getSimpleQuestions);

    std::vector<db::SimpleQuestion> dbSQuestions = getSimpleQuestions.getResult();
    std::sort(dbSQuestions.begin(),
              dbSQuestions.end(),
              [](const auto &lhs, const auto &rhs) { return lhs.name < rhs.name; });

    std::vector<io::SimpleQuestion> sQuestions;
    ::utils::transform(dbSQuestions, sQuestions, [](const auto &e) { return utils::toIO(e); });
    return sQuestions;
}
}
}