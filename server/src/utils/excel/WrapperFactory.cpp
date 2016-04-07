#include <cstdio>
#include <regex>

#include "WrapperFactory.h"
#include "consts.h"

namespace utils {
namespace excel {

namespace {
class TimeWrapper : public Wrapper {
public:
    TimeWrapper(std::int32_t h, std::int32_t m, std::int32_t s) : h(h), m(m), s(s) {
    }

    virtual void print(std::ostream &ostream) const override {
        char buf[] = "00:00:00";
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
        ostream << buf;
    }

private:
    const std::int32_t h, m, s;
};

class DateWrapper : public Wrapper {
public:
    DateWrapper(const boost::gregorian::date &date) : date(date) {
    }

    virtual void print(std::ostream &ostream) const override {
        ostream << boost::gregorian::to_iso_extended_string(date);
    }

private:
    const boost::gregorian::date date;
};

template <class T>
class SimpleWrapper : public Wrapper {
public:
    SimpleWrapper(const T &val) : val(val) {
    }

    virtual void print(std::ostream &ostream) const override {
        ostream << val;
    }

private:
    const T val;
};

template <class PTR>
class ContainerWrapper : public Wrapper {
public:
    ContainerWrapper(std::vector<PTR> &&wrappers, const std::string &delimiter)
        : delimiter(delimiter), wrappers(std::move(wrappers)) {
    }

    virtual void print(std::ostream &ostream) const override {
        auto cur = wrappers.begin();
        auto end = wrappers.end();
        while (cur != end) {
            (*cur)->print(ostream);
            if (++cur != end) {
                ostream << delimiter;
            }
        }
    }

private:
    const std::string delimiter;
    const std::vector<PTR> wrappers;
};
}

wrapper_ptr ContainerWrapperFactory::lineWrapper(std::vector<wrapper_ptr> &&cols) {
    cols.push_back(WrapperFactory::newlineWrapper());
    return rowWrapper(std::move(cols));
}

wrapper_ptr ContainerWrapperFactory::rowWrapper(std::vector<wrapper_ptr> &&cols) {
    return std::make_unique<ContainerWrapper<wrapper_ptr>>(std::move(cols), consts::delimiter);
}

wrapper_ptr ContainerWrapperFactory::colWrapper(std::vector<wrapper_ptr> &&rows) {
    return std::make_unique<ContainerWrapper<wrapper_ptr>>(std::move(rows), "\n");
}

wrapper_ptr ContainerWrapperFactory::rowWrapper(const std::vector<Wrapper *> &cols) {
    auto colsCpy = cols;
    return std::make_unique<ContainerWrapper<Wrapper *>>(std::move(colsCpy), consts::delimiter);
}

wrapper_ptr ContainerWrapperFactory::colWrapper(const std::vector<Wrapper *> &rows) {
    auto rowsCpy = rows;
    return std::make_unique<ContainerWrapper<Wrapper *>>(std::move(rowsCpy), "\n");
}

wrapper_ptr DataWrapperFactory::eventType(bool isBreak) {
    return numWrapper(isBreak);
}

wrapper_ptr DataWrapperFactory::questionOptionWrapper(bool choosen) {
    return actionWrapper(choosen);
}

wrapper_ptr DataWrapperFactory::actionWrapper(bool occured) {
    if (occured) {
        return numWrapper(1);
    } else {
        return emptyWrapper();
    }
}

wrapper_ptr DataWrapperFactory::numWrapper(std::int32_t val) {
    return std::make_unique<SimpleWrapper<std::int32_t>>(val);
}

wrapper_ptr DataWrapperFactory::timeWrapper(const utils::TimePoint &timePoint) {
    return std::make_unique<TimeWrapper>(timePoint.h, timePoint.m, timePoint.s);
}

wrapper_ptr DataWrapperFactory::dateWrapper(const boost::gregorian::date &date) {
    return std::make_unique<DateWrapper>(date);
}

wrapper_ptr HeaderWrapperFactory::exhibitActionWrapper(const std::string &actionName) {
    return stringWrapper(actionName);
}

wrapper_ptr HeaderWrapperFactory::breakActionWrapper(const std::string &actionName) {
    return stringWithPrefixWrapper(consts::header::event::breakActionPrefix, actionName);
}

wrapper_ptr HeaderWrapperFactory::simpleQuestionWrapper(Period period, const std::string &name) {
    using namespace consts::header::survey;
    auto periodStr = std::string{period == Period::Before ? before : after};
    return stringWithPrefixWrapper(periodStr + questionInfix + simplePrefix, name);
}

wrapper_ptr HeaderWrapperFactory::questionWrapper(Period period, QType type,
                                                  const std::string &name, std::size_t idx) {
    using namespace consts::header::survey;
    auto periodStr = std::string{period == Period::Before ? before : after};
    auto typeStr = std::string{type == QType::Multi ? multiChoicePrefix : sortPrefix};
    return stringWithPrefixWrapper(periodStr + questionInfix + typeStr,
                                   name + " (" + optionPrefix + std::to_string(idx) + ")");
}

wrapper_ptr LegendWrapperFactory::simpleQuestionWrapper(SimpleQType type, const std::string &name) {
    using namespace consts;
    auto t = type == SimpleQType::Num ? legend::numAns : legend::textAns;
    return stringWithPrefixWrapper(header::survey::simplePrefix, name + " (" + t + ")");
}

wrapper_ptr LegendWrapperFactory::questionWrapper(QType type, const std::string &name) {
    using namespace consts::header::survey;
    auto t = type == QType::Sort ? sortPrefix : multiChoicePrefix;
    return stringWithPrefixWrapper(t, name);
}

wrapper_ptr LegendWrapperFactory::questionOptionWrapper(QType type, const std::string &name,
                                                        std::size_t idx) {
    using namespace consts::header::survey;
    auto t = type == QType::Sort ? sortPrefix : multiChoicePrefix;
    return stringWithPrefixWrapper(t, name + " (" + optionPrefix + std::to_string(idx) + ")");
}

wrapper_ptr WrapperFactory::stringWithPrefixWrapper(const std::string &prefix,
                                                    const std::string &name) {
    return stringWrapper(prefix + consts::prefixSuffix + name);
}

wrapper_ptr WrapperFactory::newlineWrapper() {
    return stringWrapper("\n");
}

wrapper_ptr WrapperFactory::stringWrapper(const std::string &str) {
    auto strCpy = std::regex_replace(str, std::regex("\""), "''");
    strCpy = std::regex_replace(strCpy, std::regex(consts::delimiter), ",");
    return std::make_unique<SimpleWrapper<std::string>>(strCpy);
}

wrapper_ptr WrapperFactory::emptyWrapper() {
    return std::make_unique<SimpleWrapper<std::string>>(consts::emptyField);
}
}
}