#include <db/db_info.h>
#include <db/sql.h>

#include "UpdateExhibit.h"

namespace db {
namespace cmd {

UpdateExhibit::UpdateExhibit(const Exhibit &exhibit) : exhibit(exhibit) {
}

void UpdateExhibit::operator()(db::DatabaseSession &session) {
    session.execute(createUpdate());
}

std::string UpdateExhibit::createUpdate() const {
    using namespace db::info::exhibits;
    using namespace db::sql;

    auto update = Sql::update(tableName)
                      .where(C(colId) == exhibit.ID)
                      .set(colName, exhibit.name)
                      .set(colVersion, exhibit.version);
    if (exhibit.frame) {
        MapFrame frame = exhibit.frame.value();
        update.set(colMapFloor, frame.floor)
            .set(colMapFrameX, frame.x)
            .set(colMapFrameY, frame.y)
            .set(colMapFrameWidth, frame.width)
            .set(colMapFrameHeight, frame.height);
    } else {
        for (const auto frameCol :
             {colMapFrameHeight, colMapFrameWidth, colMapFrameX, colMapFrameY, colMapFloor}) {
            update.set(frameCol, Null());
        }
    }

    return update;
}
}
}