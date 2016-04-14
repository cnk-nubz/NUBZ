#ifndef REPOSITORY__EXHIBITS__H
#define REPOSITORY__EXHIBITS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

/*
 * Validation:
 * Every method validates `ID`, `name`, `rgbHex`, `frame`
 * and therefore can throw InvalidData or DuplicateName
 *
 * only `incReferenceCount` and `decReferenceCount` can opperate on deleted exhibits
 *
 * Versioning:
 * `insert`, `set*` cause version update
 * `remove` causes version and lastDeletedVersion update
 *
 * Deletion:
 * `remove` makes exhibit invisible to the user, but it will stay in database
 * `refresh` enforces to actually delete removed exhibits from database if reference count == 0
 */
class Exhibits {
public:
    struct Exhibit {
        struct Frame {
            std::int32_t x;
            std::int32_t y;
            std::int32_t width;
            std::int32_t height;
            std::int32_t floor;
        };

        std::int32_t ID;
        std::string name;
        std::int32_t rgbHex;
        std::int32_t version;
        boost::optional<Frame> frame;
    };

    Exhibits(db::DatabaseSession &session);

    std::int32_t getCurrentVersion();
    std::int32_t getLastDeletedVersion();

    void incReferenceCount(std::int32_t ID);
    void decReferenceCount(std::int32_t ID);

    void refresh();

    // non-throwing get
    boost::optional<Exhibit> getOpt(std::int32_t ID);

    Exhibit get(std::int32_t ID);
    std::vector<std::int32_t> getAllIDs();
    std::vector<Exhibit> getAll();
    std::vector<Exhibit> getAllNewerThan(std::int32_t version);

    void insert(Exhibit *exhibit);

    void setRgbHex(std::int32_t ID, std::int32_t newRgbHex);
    void setFrame(std::int32_t ID, const boost::optional<Exhibit::Frame> &newFrame);
    void resetFrames(std::int32_t floor);

    void remove(std::int32_t ID);

private:
    void updateReferenceCount(std::int32_t ID, std::int32_t diff);

    void checkID(std::int32_t ID);
    void checkName(const std::string &name);
    void checkFrame(const Exhibit::Frame &frame);
    void checkRgbHex(std::int32_t rgbHex);

    db::DatabaseSession &session;
};

using Exhibit = Exhibits::Exhibit;
}

#endif
