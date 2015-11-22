#ifndef ABSTRACT_COMMAND__H
#define ABSTRACT_COMMAND__H

#include <stdexcept>

namespace command {
    template <class InputT, class OutputT>
    class AbstractCommand {
    public:
        virtual ~AbstractCommand() = default;
        virtual OutputT perform(const InputT &input) = 0;
    };

    template <class InputT>
    class AbstractCommandOnlyIn {
    public:
        virtual ~AbstractCommandOnlyIn() = default;
        virtual void perform(const InputT &input) = 0;
    };

    using InvalidInput = std::logic_error;
}

#endif