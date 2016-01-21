#include <cassert>

#include "commons.h"

rapidjson::Document parseJson(const std::string &str) {
    rapidjson::Document json;
    json.Parse(str.c_str());
    return json;
}

std::string jsonToString(const rapidjson::Value &json) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    return std::string(buffer.GetString());
}

std::vector<std::int32_t> parseIntArray(const rapidjson::Value &json) {
    return parseArray(json, [](const rapidjson::Value &jsonVal) {
        assert(jsonVal.IsInt());
        return (std::int32_t)jsonVal.GetInt();
    });
}

rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str) {
    return rapidjson::GenericStringRef<char>(str.c_str());
}

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}

const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}