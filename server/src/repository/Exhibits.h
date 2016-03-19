#ifndef REPOSITORY__EXHIBITS__H
#define REPOSITORY__EXHIBITS__H

#include <cstdint>
#include <string>

#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

namespace repository {

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

    std::vector<std::int32_t> getAllIDs();

    boost::optional<Exhibit> get(std::int32_t ID);

    // throws InvalidData in case of non existing ID
    Exhibit getF(std::int32_t ID);

    std::vector<Exhibit> getAll();
    std::vector<Exhibit> getAllNewerThan(std::int32_t version);

    void remove(std::int32_t ID);
    void removeAll();

    // ID will be saved in the given struct
    // throws InvalidData in case of incorrect frame
    void insert(Exhibit *exhibit);
    void insert(std::vector<Exhibit> *exhibits);

    // throws InvalidData in case of incorrect frame
    void setFrame(std::int32_t ID, const boost::optional<Exhibit::Frame> &newFrame);

    void setVersion(std::int32_t ID, std::int32_t newVersion);

    // throws InvalidData in case of incorrect rgb value
    void setRgbHex(std::int32_t ID, std::int32_t newRgbHex);

private:
    void checkFrame(const Exhibit::Frame &frame);
    void checkRgbHex(const std::int32_t rgbHex);

    db::DatabaseSession &session;
};

using Exhibit = Exhibits::Exhibit;
}

#endif
