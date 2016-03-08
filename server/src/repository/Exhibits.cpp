#include <utils/fp_algorithm.h>

#include <db/table/Exhibits.h>

#include "DefaultRepo.h"
#include "Exhibits.h"

namespace repository {

using Table = db::table::Exhibits;
using Impl = repository::detail::DefaultRepoWithID<Table>;

namespace {
Table::Row toDB(const Exhibits::Exhibit &exhibit);
Exhibits::Exhibit fromDB(const Table::Row &exhibit);
}

Exhibits::Exhibits(db::DatabaseSession &session) : session(session) {
}

std::vector<std::int32_t> Exhibits::getAllIDs() {
    return Impl::getAllIDs(session);
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
    auto sql = Table::select().where(Table::colVersion > version);
    auto dbTuples = session.getResults(sql);

    auto result = std::vector<Exhibit>{};
    utils::transform(
        dbTuples, result, [](auto &tuple) { return fromDB(Table::RowFactory::fromDB(tuple)); });
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
    exhibit->ID = Impl::insert(session, toDB(*exhibit));
}

void Exhibits::setVersion(std::int32_t ID, std::int32_t newVersion) {
    auto sql = Table::update().set(Table::colVersion, newVersion).where(Table::colId == ID);
    session.execute(sql);
}

void Exhibits::setFrame(std::int32_t ID, const boost::optional<Exhibit::Frame> &newFrame) {
    auto x = boost::optional<std::int32_t>{};
    auto y = boost::optional<std::int32_t>{};
    auto width = boost::optional<std::int32_t>{};
    auto height = boost::optional<std::int32_t>{};
    auto floor = boost::optional<std::int32_t>{};
    if (newFrame) {
        auto frame = newFrame.value();
        x = frame.x;
        y = frame.y;
        width = frame.width;
        height = frame.height;
        floor = frame.floor;
    }

    auto sql = Table::update()
                   .where(Table::colId == ID)
                   .set(Table::colFrameX, x)
                   .set(Table::colFrameY, y)
                   .set(Table::colFrameWidth, width)
                   .set(Table::colFrameHeight, height)
                   .set(Table::colFrameFloor, floor);
    session.execute(sql);
}

namespace {
Table::Row toDB(const Exhibits::Exhibit &exhibit) {
    auto row = Table::Row{};
    row.name = exhibit.name;
    row.version = exhibit.version;
    if (auto frame = exhibit.frame) {
        row.frameX = frame.value().x;
        row.frameY = frame.value().y;
        row.frameWidth = frame.value().width;
        row.frameHeight = frame.value().height;
        row.frameFloor = frame.value().floor;
    }
    return row;
}

Exhibits::Exhibit fromDB(const Table::Row &row) {
    auto exhibit = Exhibits::Exhibit{};
    exhibit.ID = row.ID;
    exhibit.name = row.name;
    exhibit.version = row.version;

    auto frameData = std::vector<boost::optional<std::int32_t>>{
        row.frameX, row.frameY, row.frameWidth, row.frameHeight, row.frameFloor};
    if (::utils::all_of(frameData, [](auto &opt) { return (bool)opt; })) {
        auto frame = Exhibit::Frame{};
        frame.x = row.frameX.value();
        frame.y = row.frameY.value();
        frame.width = row.frameWidth.value();
        frame.height = row.frameHeight.value();
        frame.floor = row.frameFloor.value();
        exhibit.frame = frame;
    }
    return exhibit;
}
}
}