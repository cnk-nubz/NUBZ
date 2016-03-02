#ifndef DB_FACTORY_JSON__COMMONS__H
#define DB_FACTORY_JSON__COMMONS__H

#include <string>
#include <type_traits>
#include <vector>

#include <boost/optional.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path);
const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path);

std::int32_t parseInt(const rapidjson::Value &json);
std::string parseString(const rapidjson::Value &json);
std::vector<std::int32_t> parseIntArray(const rapidjson::Value &json);
std::vector<std::string> parseStringArray(const rapidjson::Value &json);

template <class Parser>
auto parseArray(Parser &&parser, const rapidjson::Value &json)
    -> std::vector<std::result_of_t<Parser(const rapidjson::Value &)>> {
    assert(json.IsArray());
    std::vector<std::result_of_t<Parser(const rapidjson::Value &)>> result;
    for (rapidjson::SizeType i = 0; i < json.Size(); i++) {
        result.push_back(parser(json[i]));
    }
    return result;
}

rapidjson::Document parseJson(const std::string &str);
std::string jsonToString(const rapidjson::Value &json);
rapidjson::GenericStringRef<char> toStrAdapter(const std::string &str);

template <class Parser>
auto parse(Parser &&parser, const rapidjson::Value &root, const std::string &path)
    -> std::result_of_t<Parser(const rapidjson::Value &)> {
    return parser(getNode(root, path));
}

template <class Parser>
auto parseOpt(Parser &&parser, const rapidjson::Value &root, const std::string &path)
    -> boost::optional<std::result_of_t<Parser(const rapidjson::Value &)>> {
    if (!root.HasMember(path.c_str())) {
        return {};
    }
    return parser(getNode(root, path));
}

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
