#include <db/db_info.h>

#include <boost/lexical_cast.hpp>

#include "ExperimentFactory.h"
#include "commons.h"

namespace db {
namespace factory {
namespace json {

const std::vector<std::string> &ExperimentFactory::fieldsOrder() noexcept {
    using namespace db::info::experiments;
    static const std::vector<std::string> order = {colId, colName, colDocument};
    return order;
}

std::string ExperimentFactory::createJson(const Experiment &experiment) {
    using namespace db::info::experiments;

    rapidjson::Document document;
    auto &allocator = document.GetAllocator();

    auto actionsKey = toStupidStringAdapter(fieldActions);
    auto breakActionsKey = toStupidStringAdapter(fieldBreakActions);
    auto actionsJson = createTrivialArray(experiment.actions, allocator);
    auto breakActionsJson = createTrivialArray(experiment.breakActions, allocator);

    rapidjson::Value json(rapidjson::kObjectType);
    json.AddMember(actionsJson, actionsKey, allocator);
    json.AddMember(breakActionsJson, breakActionsKey, allocator);

    return jsonToString(json);
}

Experiment ExperimentFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    using namespace db::info::experiments;

    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    Experiment experiment;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), experiment.ID));
    experiment.name = raw[1].value();

    rapidjson::Document json = parseJson(raw[2].value());
    experiment.actions = parseIntArray(getNode(json, fieldActions));
    experiment.breakActions = parseIntArray(getNode(json, fieldBreakActions));
    return experiment;
}
}
}
}