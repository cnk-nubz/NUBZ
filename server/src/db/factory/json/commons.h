#ifndef DB_FACTORY_JSON__COMMONS__H
#define DB_FACTORY_JSON__COMMONS__H

#include <string>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path);
const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path);
rapidjson::Document parseJson(const std::string &str);
std::string jsonToString(const rapidjson::Value &json);
rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str);

template <class Parser>
auto parseArray(const rapidjson::Value &json, Parser &&parser)
    -> std::vector<std::result_of_t<Parser(const rapidjson::Value &)>> {
    assert(json.IsArray());

    std::vector<std::result_of_t<Parser(const rapidjson::Value &)>> result;
    for (rapidjson::SizeType i = 0; i < json.Size(); i++) {
        result.push_back(parser(json[i]));
    }
    return result;
}

std::vector<std::int32_t> parseIntArray(const rapidjson::Value &json);

// Translator = rapidjson::Value(const Raw &, rapidjson::Document::AllocatorType &)
template <class Raw, class Translator>
rapidjson::Value createArray(const std::vector<Raw> &rawArr,
                             rapidjson::Document::AllocatorType &allocator,
                             Translator &&translator) {
    rapidjson::Value json(rapidjson::kArrayType);
    for (const auto &raw : rawArr) {
        json.PushBack(translator(raw, allocator), allocator);
    }
    return json;
}

template <class T>
rapidjson::Value createTrivialArray(const std::vector<T> &rawData,
                                    rapidjson::Document::AllocatorType &allocator) {
    return createArray(
        rawData, allocator, [](const T &raw, auto &) { return rapidjson::Value{raw}; });
}

#endif
