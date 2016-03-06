#ifndef DB_JSON__JSON__H
#define DB_JSON__JSON__H

#include <string>
#include <type_traits>
#include <vector>

#include <boost/optional.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace db {

// parse
namespace json {

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path);
const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path);
rapidjson::GenericStringRef<char> toStrAdapter(const std::string &str);

rapidjson::Document parseJson(const std::string &str);

std::int32_t parseInt(const rapidjson::Value &json);
std::string parseString(const rapidjson::Value &json);

std::vector<std::int32_t> parseIntArray(const rapidjson::Value &json);
std::vector<std::string> parseStringArray(const rapidjson::Value &json);

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

template <class Parser>
auto parseOpt(const rapidjson::Value &root, const std::string &path, Parser &&parser)
    -> boost::optional<std::result_of_t<Parser(const rapidjson::Value &)>> {
    if (!root.HasMember(path.c_str())) {
        return {};
    }
    return parser(getNode(root, path));
}
}

// create
namespace json {
std::string jsonToString(const rapidjson::Value &json);

template <class F>
auto bindAllocator(rapidjson::Document::AllocatorType &allocator, F &&f) {
    return std::bind(std::forward<F>(f), std::ref(allocator), std::placeholders::_1);
}

rapidjson::Value createStringArray(rapidjson::Document::AllocatorType &allocator,
                                   const std::vector<std::string> &rawData);

// Translator = rapidjson::Value(rapidjson::Document::AllocatorType &, const Raw &)
template <class Raw, class Translator>
rapidjson::Value createArray(rapidjson::Document::AllocatorType &allocator,
                             const std::vector<Raw> &rawArr, Translator &&translator) {
    rapidjson::Value json(rapidjson::kArrayType);
    for (const auto &raw : rawArr) {
        json.PushBack(translator(allocator, raw), allocator);
    }
    return json;
}

template <class T>
rapidjson::Value createTrivialArray(rapidjson::Document::AllocatorType &allocator,
                                    const std::vector<T> &rawData) {
    return createArray(
        allocator, rawData, [](auto &, const T &raw) { return rapidjson::Value{raw}; });
}

namespace detail {
struct ValueAdapter {
    ValueAdapter(rapidjson::Value &val) : ptr(&val){};
    rapidjson::Value *ptr;
};
}

template <class... Args>
rapidjson::Value createDictionary(rapidjson::Document::AllocatorType &allocator,
                                  std::pair<std::string, Args>... entries) {
    // boxing rapidjson::Value into detail::ValueAdapter to prevent copying
    auto values = std::vector<std::pair<std::string, detail::ValueAdapter>>{entries...};
    auto json = rapidjson::Value{rapidjson::kObjectType};
    for (auto &val : values) {
        json.AddMember(toStrAdapter(val.first), *val.second.ptr, allocator);
    }
    return json;
}
}
}

#endif
