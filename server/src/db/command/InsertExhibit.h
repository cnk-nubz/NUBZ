#ifndef DB_CMD__INSERT_EXHIBIT__H
#define DB_CMD__INSERT_EXHIBIT__H

#include <db/DatabaseSession.h>
#include <db/struct/Exhibit.h>

namespace db {
namespace cmd {

class InsertExhibit {
public:
    InsertExhibit(const Exhibit &exhibit);

    std::int32_t operator()(DatabaseSession &session);
    std::int32_t getId() const;

private:
    const Exhibit exhibit;
    std::int32_t exhibitId;

    std::string createInsert() const;
};
}
}

#endif