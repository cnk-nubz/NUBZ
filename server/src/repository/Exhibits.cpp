#include <utils/fp_algorithm.h>

#include <db/table/Exhibits.h>

#include "DefaultRepo.h"
#include "Exhibits.h"
#include "MapImages.h"
#include "error/InvalidData.h"

namespace repository {

using Table = db::table::Exhibits;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
struct OptFrame {
    OptFrame(const boost::optional<Exhibits::Exhibit::Frame> &frame);

    boost::optional<std::int32_t> x;
    boost::optional<std::int32_t> y;
    boost::optional<std::int32_t> width;
    boost::optional<std::int32_t> height;
    boost::optional<std::int32_t> floor;
};

Table::Sql::in_t toDB(const Exhibits::Exhibit &exhibit);
Exhibits::Exhibit fromDB(const Table::Sql::out_t &exhibit);
}

Exhibits::Exhibits(db::DatabaseSession &session) : session(session) {
}

std::vector<std::int32_t> Exhibits::getAllIDs() {
    return Impl::getAllIDs(session);
}

Exhibit Exhibits::getF(std::int32_t ID) {
    if (auto res = get(ID)) {
        return res.value();
    } else {
        throw InvalidData{"invalid exhibit ID"};
    }
}

boost::optional<Exhibits::Exhibit> Exhibits::get(std::int32_t ID) {
    if (auto res = Impl::get(session, ID)) {
        return fromDB(res.value());
    } else {
        return {};
    }
}

std::vector<Exhibits::Exhibit> Exhibits::getAll() {
    auto result = std::vector<Exhibits::Exhibit>{};
    utils::transform(Impl::getAll(session), result, fromDB);
    return result;
}

std::vector<Exhibit> Exhibits::getAllNewerThan(std::int32_t version) {
    auto sql = Table::Sql::select().where(Table::Version > version);
    auto dbTuples = session.getResults(sql);

    auto result = std::vector<Exhibit>{};
    utils::transform(dbTuples, result, fromDB);
    return result;
}

void Exhibits::remove(std::int32_t ID) {
    Impl::remove(session, ID);
}

void Exhibits::removeAll() {
    Impl::removeAll(session);
}

void Exhibits::insert(std::vector<Exhibits::Exhibit> *exhibits) {
    assert(exhibits);
    for (auto &exhibit : *exhibits) {
        insert(&exhibit);
    }
}

void Exhibits::insert(Exhibits::Exhibit *exhibit) {
    assert(exhibit);
    if (exhibit->frame) {
        checkFrame(exhibit->frame.value());
    }
    checkRgbHex(exhibit->rgbHex);

    exhibit->ID = Impl::insert(session, toDB(*exhibit));
}

void Exhibits::setRgbHex(std::int32_t ID, std::int32_t newRgbHex) {
    if (!get(ID)) {
        throw InvalidData{"incorrect exhibit ID"};
    }
    checkRgbHex(newRgbHex);

    auto sql = Table::Sql::update().set(Table::RgbHex, newRgbHex).where(Table::ID == ID);
    session.execute(sql);
}

void Exhibits::checkRgbHex(std::int32_t rgbHex) {
    if ((rgbHex & 0x00FFFFFF) != rgbHex) {
        throw InvalidData{"incorrect rgbHex value"};
    }
}

void Exhibits::setVersion(std::int32_t ID, std::int32_t newVersion) {
    if (!get(ID)) {
        throw InvalidData{"incorrect exhibit ID"};
    }
    auto sql = Table::Sql::update().set(Table::Version, newVersion).where(Table::ID == ID);
    session.execute(sql);
}

void Exhibits::setFrame(std::int32_t ID, const boost::optional<Exhibit::Frame> &newFrame) {
    if (newFrame) {
        checkFrame(newFrame.value());
    }
    if (!get(ID)) {
        throw InvalidData{"incorrect exhibit ID"};
    }

    auto frame = OptFrame{newFrame};

    auto sql = Table::Sql::update()
                   .where(Table::ID == ID)
                   .set(Table::FrameX, frame.x)
                   .set(Table::FrameY, frame.y)
                   .set(Table::FrameWidth, frame.width)
                   .set(Table::FrameHeight, frame.height)
                   .set(Table::FrameFloor, frame.floor);
    session.execute(sql);
}

void Exhibits::checkFrame(const Exhibit::Frame &frame) {
    if (auto mapImage = MapImages{session}.get(frame.floor)) {
        if (frame.x + frame.width > mapImage.value().width ||
            frame.y + frame.height > mapImage.value().height || frame.x < 0 || frame.y < 0) {
            throw InvalidData{"incorrect exhibit frame - out of bounds"};
        }
    } else {
        throw InvalidData{"incorrect exhibit frame - given floor doesn't exist"};
    }
}

void Exhibits::resetFrames(std::int32_t floor, std::int32_t newVersion) {
    using namespace db::sql;
    auto sql = Table::Sql::update()
                   .where(Table::FrameFloor == floor)
                   .set(Table::Version, newVersion)
                   .set(Table::FrameX, Null)
                   .set(Table::FrameY, Null)
                   .set(Table::FrameWidth, Null)
                   .set(Table::FrameHeight, Null)
                   .set(Table::FrameFloor, Null);
    session.execute(sql);
}

namespace {
OptFrame::OptFrame(const boost::optional<Exhibits::Exhibit::Frame> &optFrame) {
    if (optFrame) {
        auto frame = optFrame.value();
        x = frame.x;
        y = frame.y;
        width = frame.width;
        height = frame.height;
        floor = frame.floor;
    }
}

Table::Sql::in_t toDB(const Exhibits::Exhibit &exhibit) {
    auto frame = OptFrame{exhibit.frame};

    return std::make_tuple(Table::FieldName{exhibit.name},
                           Table::FieldVersion{exhibit.version},
                           Table::FieldRgbHex{exhibit.rgbHex},
                           Table::FieldFrameX{frame.x},
                           Table::FieldFrameY{frame.y},
                           Table::FieldFrameWidth{frame.width},
                           Table::FieldFrameHeight{frame.height},
                           Table::FieldFrameFloor{frame.floor});
}

Exhibits::Exhibit fromDB(const Table::Sql::out_t &exhibit) {
    auto result = Exhibits::Exhibit{};
    result.ID = std::get<Table::FieldID>(exhibit).value;
    result.name = std::get<Table::FieldName>(exhibit).value;
    result.version = std::get<Table::FieldVersion>(exhibit).value;
    result.rgbHex = std::get<Table::FieldRgbHex>(exhibit).value;

    auto x = std::get<Table::FieldFrameX>(exhibit).value;
    auto y = std::get<Table::FieldFrameY>(exhibit).value;
    auto width = std::get<Table::FieldFrameWidth>(exhibit).value;
    auto height = std::get<Table::FieldFrameHeight>(exhibit).value;
    auto floor = std::get<Table::FieldFrameFloor>(exhibit).value;
    if (x && y && width && height && floor) {
        auto frame = Exhibits::Exhibit::Frame{};
        frame.x = x.value();
        frame.y = y.value();
        frame.width = width.value();
        frame.height = height.value();
        frame.floor = floor.value();
        result.frame = frame;
    }

    return result;
}
}
}
