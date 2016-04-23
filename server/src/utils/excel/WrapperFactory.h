#ifndef UTILS_EXCEL__WRAPPED_FACTORY__H
#define UTILS_EXCEL__WRAPPED_FACTORY__H

#include <cstdint>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>

#include <utils/TimePoint.h>

#include <repository/Reports.h>

#include "Wrapper.h"

namespace utils {
namespace excel {

struct ContainerWrapperFactory {
    static wrapper_ptr rowWrapper(std::vector<wrapper_ptr> &&cols);
    static wrapper_ptr lineWrapper(std::vector<wrapper_ptr> &&cols);
    static wrapper_ptr colWrapper(std::vector<wrapper_ptr> &&rows);

    // weak version
    static wrapper_ptr rowWrapper(const std::vector<Wrapper *> &cols);
    static wrapper_ptr colWrapper(const std::vector<Wrapper *> &rows);
};

struct WrapperFactory {
    static wrapper_ptr newlineWrapper();
    static wrapper_ptr stringWrapper(const std::string &str);
    static wrapper_ptr emptyWrapper();

protected:
    static wrapper_ptr stringWithPrefixWrapper(const std::string &prefix, const std::string &name);
};

struct DataWrapperFactory : WrapperFactory {
    static wrapper_ptr numWrapper(std::int32_t val);
    static wrapper_ptr timeWrapper(const ::utils::TimePoint &timePoint);
    static wrapper_ptr dateWrapper(const boost::gregorian::date &date);
    static wrapper_ptr actionWrapper(bool occured);
    static wrapper_ptr questionOptionWrapper(bool choosen);
    static wrapper_ptr eventType(bool isBreak);
};

struct HeaderWrapperFactory : WrapperFactory {
    static wrapper_ptr exhibitActionWrapper(const std::string &actionName);
    static wrapper_ptr breakActionWrapper(const std::string &actionName);

    enum class Period { Before, After };
    static wrapper_ptr simpleQuestionWrapper(Period period, const std::string &name);

    enum class QType { Multi, Sort };
    static wrapper_ptr legendQuestionWrapper(QType type, const std::string &name);
    static wrapper_ptr legendQuestionOptionWrapper(QType type, const std::string &name,
                                                   std::size_t idx);
    static wrapper_ptr questionWrapper(Period period, QType type, const std::string &name,
                                       std::size_t idx);
};

struct LegendWrapperFactory : WrapperFactory {
    enum class QType { Multi, Sort };
    enum class SimpleQType { Text, Num };

    static wrapper_ptr simpleQuestionWrapper(SimpleQType type, const std::string &name);
    static wrapper_ptr questionWrapper(QType type, const std::string &name);
    static wrapper_ptr questionOptionWrapper(QType type, const std::string &name, std::size_t idx);
};
}
}

#endif