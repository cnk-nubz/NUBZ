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
    return parseArray(json,
                      [](const rapidjson::Value &jsonVal) {
                          assert(jsonVal.IsInt());
                          return (std::int32_t)jsonVal.GetInt();
                      });
}

std::vector<std::string> parseStringArray(const rapidjson::Value &json) {
    return parseArray(json,
                      [](const rapidjson::Value &jsonVal) {
                          assert(jsonVal.IsString());
                          return std::string(jsonVal.GetString());
                      });
}

rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str) {
    return rapidjson::GenericStringRef<char>(str.c_str());
}

std::int32_t parseInt(const rapidjson::Value &root, const std::string &path) {
    auto optRes = parseOptInt(root, path);
    assert(optRes);
    return optRes.value();
}

std::string parseString(const rapidjson::Value &root, const std::string &path) {
    auto optRes = parseOptString(root, path);
    assert(optRes);
    return optRes.value();
}

boost::optional<std::int32_t> parseOptInt(const rapidjson::Value &root, const std::string &path) {
    if (!root.HasMember(path.c_str())) {
        return {};
    }
    return getNode(root, path).GetInt();
}

boost::optional<std::string> parseOptString(const rapidjson::Value &root, const std::string &path) {
    if (!root.HasMember(path.c_str())) {
        return {};
    }
    return std::string(getNode(root, path).GetString());
}

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}

const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}