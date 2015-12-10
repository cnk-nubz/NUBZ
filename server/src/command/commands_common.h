#ifndef COMMAND__COMMANDS_COMMON__H
#define COMMAND__COMMANDS_COMMON__H

#define DEFAULT_CP_MV(ClassName)                      \
    ClassName(const ClassName &) = delete;            \
    ClassName(ClassName &&) = default;                \
    ClassName &operator=(const ClassName &) = delete; \
    ClassName &operator=(ClassName &&) = default

#endif