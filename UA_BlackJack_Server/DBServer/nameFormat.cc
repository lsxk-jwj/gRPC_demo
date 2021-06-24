#include "nameFormat.h"

#include <cstdio>
#include <cstring>

static char buffer[64]; // output format buffer

char* ua_black_jack_server::data_base_server::nameFormat::GetKey(FormatType type, int64_t input) {
    const auto ret = snprintf(buffer, sizeof buffer, "%ld", input);
    // TODO: check the return value
    const auto next = buffer + ret;
    switch (type) {
    case FormatType::UID_TO_NICKNAME:
        strcpy(next, ":name");
        break;
    case FormatType::UID_TO_PASSWORD:
        strcpy(next, ":pass");
        break;
    case FormatType::UID_TO_SCORE:
        strcpy(next, ":score");
        break;
    case FormatType::UID_TO_FRIEND_LIST:
        strcpy(next, ":friend");
        break;
    case FormatType::UID_TO_WAIT_FRIEND_LIST:
        strcpy(next, ":waitFriend");
        break;
    case FormatType::UID_TO_MATCH_LIST:
        strcpy(next, ":matches");
        break;
    case FormatType::MATCH_ID_TO_MATCH_HISTORY:
        strcpy(next, ":match");
        break;
    default:
        return nullptr;
    }
    return buffer;
}

char* ua_black_jack_server::data_base_server::nameFormat::GetKey(FormatType type, const char* input) {
    if (type == FormatType::NICKNAME_TO_UID && input) {
        const auto len = strlen(input);
        std::strcpy(buffer, input);
        strcpy(buffer + len, ":uid");
        return buffer;
    }
    return nullptr;
}

char* ua_black_jack_server::data_base_server::nameFormat::GetKey(FormatType type) {
    switch (type) {
    case FormatType::GET_NEXT_UID:
        strcpy(buffer, "UID");
        break;
    case FormatType::RANK:
        strcpy(buffer, "RANK");
        break;
    default:
        return nullptr;
    }
    return buffer;
}
