#ifndef ABSTRACT_COMMAND__H
#define ABSTRACT_COMMAND__H

namespace command {
    template <class InputT, class OutputT>
    class AbstractCommand {
    public:
        virtual ~AbstractCommand() = default;
        virtual OutputT perform(const InputT &input) = 0;
    };
}

#endif