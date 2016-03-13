#ifndef SERVER_IO__QUESTION__H
#define SERVER_IO__QUESTION__H

#include <cstdint>
#include <string>
#include <tuple>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace output {

struct Question {
    std::int32_t ID;
    std::string name;
    std::string question;

#define CMP_OP(op)                                              \
    bool operator op(const Question &rhs) const {               \
        return std::make_tuple(name, question, ID)              \
            op std::make_tuple(rhs.name, rhs.question, rhs.ID); \
    }

    CMP_OP(<)
    CMP_OP(>)
    CMP_OP(<=)
    CMP_OP(>=)
    CMP_OP(==)
    CMP_OP(!=)

#undef CMP_OP
};
}
}
}

#endif