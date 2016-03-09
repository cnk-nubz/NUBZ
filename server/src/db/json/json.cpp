#include "json.h"

namespace db {
namespace json {

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}

const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}

rapidjson::GenericStringRef<char> toStrAdapter(const char *str) {
    return rapidjson::GenericStringRef<char>(str);
}

rapidjson::Document parseJson(const std::string &str) {
    rapidjson::Document json;
    json.Parse(str.c_str());
    return json;
}

std::vector<std::int32_t> parseIntArray(const rapidjson::Value &json) {
    return parseArray(json, parseInt);
}

std::vector<std::string> parseStringArray(const rapidjson::Value &json) {
    return parseArray(json, parseString);
}

std::int32_t parseInt(const rapidjson::Value &json) {
    assert(json.IsInt());
    return static_cast<std::int32_t>(json.GetInt());
}

std::string parseString(const rapidjson::Value &json) {
    assert(json.IsString());
    return json.GetString();
}

std::string jsonToString(const rapidjson::Value &json) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    return std::string(buffer.GetString());
}

rapidjson::Value createIntArray(rapidjson::Document::AllocatorType &allocator,
                                const std::vector<std::int32_t> &rawData) {
    return createArray(
        allocator, rawData, [](auto &, const std::int32_t &raw) { return createInt(raw); });
}

rapidjson::Value createStringArray(rapidjson::Document::AllocatorType &allocator,
                                   const std::vector<std::string> &rawData) {
    return createArray(
        allocator, rawData, [](auto &, const std::string &raw) { return createString(raw); });
}

rapidjson::Value createInt(std::int32_t raw) {
    return rapidjson::Value{raw};
}

rapidjson::Value createString(const std::string &raw) {
    return rapidjson::Value{toStrAdapter(raw.c_str())};
}
}
}