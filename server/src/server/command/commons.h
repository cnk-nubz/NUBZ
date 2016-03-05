#ifndef SERVER_COMMAND__COMMANDS_COMMON__H
#define SERVER_COMMAND__COMMANDS_COMMON__H

#define SRV_CMD_CP_MV(ClassName)                      \
    ClassName(const ClassName &) = delete;            \
    ClassName(ClassName &&) = default;                \
    ClassName &operator=(const ClassName &) = delete; \
    ClassName &operator=(ClassName &&) = default

#endif