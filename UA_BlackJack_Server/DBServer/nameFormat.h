#pragma once
#include <cstdint>

namespace ua_black_jack_server {namespace data_base_server {namespace nameFormat {

enum class FormatType {
    UID_TO_NICKNAME,
    UID_TO_PASSWORD,
    UID_TO_SCORE,
    UID_TO_FRIEND_LIST,
    UID_TO_MATCH_LIST,
    UID_TO_WAIT_FRIEND_LIST,
    NICKNAME_TO_UID,
    RANK,
    MATCH_ID_TO_MATCH_HISTORY,
    GET_NEXT_UID,
};

char* GetKey(FormatType type, int64_t input);

inline char* GetKey(FormatType type, int32_t input) {
    return GetKey(type, static_cast<int64_t>(input));
}

char* GetKey(FormatType type, const char* input);

char* GetKey(FormatType type);

}}}
