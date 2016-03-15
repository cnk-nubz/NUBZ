#include <server/io/InvalidInput.h>

#include "SaveRawReportCommand.h"

namespace server {
namespace command {

SaveRawReportCommand::SaveRawReportCommand(db::Database &db) : db(db) {
}

void SaveRawReportCommand::operator()(const io::input::RawReport &input) {
    db.execute([&](db::DatabaseSession &session) {
        auto repo = repository::Reports{session};
        if (!repo.exist(input.ID)) {
            repo.insert(input.toRepo());
        }
    });
}
}
}