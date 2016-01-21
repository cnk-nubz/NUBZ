#include <vector>

#include <boost/lexical_cast.hpp>

#include "RawReportFactory.h"
#include "commons.h"

// to json
namespace db {
namespace factory {
namespace json {

rapidjson::Value createEvent(const db::RawReport::Event &event,
                             rapidjson::Document::AllocatorType &allocator);

std::string RawReportFactory::createJson(const db::RawReport &report) {
    rapidjson::Document document;
    auto &allocator = document.GetAllocator();
    const auto historyKey = toStupidStringAdapter(db::info::reports::field0History);
    auto jsonHistory = createArray<db::RawReport::Event>(report.history, allocator, createEvent);

    rapidjson::Value json(rapidjson::kObjectType);
    json.AddMember(historyKey, jsonHistory, allocator);
    return jsonToString(json);
}

rapidjson::Value createEvent(const db::RawReport::Event &event,
                             rapidjson::Document::AllocatorType &allocator) {
    using namespace db::info::reports;
    rapidjson::Value json(rapidjson::kObjectType);

    const auto exhibitIdKey = toStupidStringAdapter(field1ExhibitId);
    const auto durationKey = toStupidStringAdapter(field1DurationInSecs);
    const auto actionsKey = toStupidStringAdapter(field1Actions);

    if (event.exhibitId) {
        json.AddMember(exhibitIdKey, rapidjson::Value(event.exhibitId.value()), allocator);
    }

    json.AddMember(durationKey, rapidjson::Value(event.durationInSecs), allocator);
    auto jsonActions = createTrivialArray<std::int32_t>(event.actions, allocator);
    json.AddMember(actionsKey, jsonActions, allocator);

    return json;
}
}
}
}

// from json
namespace db {
namespace factory {
namespace json {

RawReport::Event parseEvent(const rapidjson::Value &jsonEvent);

const std::vector<std::string> &RawReportFactory::fieldsOrder() noexcept {
    using namespace db::info::reports;
    static const std::vector<std::string> order = {colId, colDocument};
    return order;
}

RawReport RawReportFactory::create(const std::vector<boost::optional<std::string>> &raw) noexcept {
    using namespace db::info::reports;

    assert(raw.size() == fieldsOrder().size());
    assert(raw[0] && raw[1]);

    RawReport report;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), report.ID));
    rapidjson::Document json = parseJson(raw[1].value());

    const auto &history = getNode(json, field0History);
    report.history = parseArray(history, parseEvent);

    return report;
}

RawReport::Event parseEvent(const rapidjson::Value &jsonEvent) {
    using namespace db::info::reports;

    assert(jsonEvent.IsObject());

    const auto &duration = getNode(jsonEvent, field1DurationInSecs);
    const auto &actions = getNode(jsonEvent, field1Actions);

    RawReport::Event event;
    assert(duration.IsInt());
    event.durationInSecs = duration.GetInt();
    event.actions = parseIntArray(actions);

    if (jsonEvent.HasMember(field1ExhibitId.c_str())) {
        const auto &exhibitId = getNode(jsonEvent, field1ExhibitId);
        assert(exhibitId.IsInt());
        event.exhibitId = exhibitId.GetInt();
    }

    return event;
}
}
}
}