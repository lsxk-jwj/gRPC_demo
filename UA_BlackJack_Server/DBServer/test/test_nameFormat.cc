#include "../nameFormat.h"

#include <gtest/gtest.h>

using ua_black_jack_server::data_base_server::nameFormat::GetKey;
using ua_black_jack_server::data_base_server::nameFormat::FormatType;



TEST(nameFormat, GetKeyForInt64_t) {
    constexpr int64_t uid = 6657;
    constexpr int     mid = 9638;
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_NICKNAME, uid), "6657:name"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_PASSWORD, uid), "6657:pass"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_SCORE, uid), "6657:score"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_FRIEND_LIST, uid), "6657:friend"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_WAIT_FRIEND_LIST, uid), "6657:waitFriend"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::UID_TO_MATCH_LIST, uid), "6657:matches"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::MATCH_ID_TO_MATCH_HISTORY, mid), "9638:match"));
}

TEST(nameFormat, GetKeyForVoid) {
    EXPECT_FALSE(strcmp(GetKey(FormatType::RANK), "RANK"));
    EXPECT_FALSE(strcmp(GetKey(FormatType::GET_NEXT_UID), "UID"));
}

TEST(nameFormat, GetKeyForStr) {
    const char* nickName = "owen";
    EXPECT_FALSE(strcmp(GetKey(FormatType::NICKNAME_TO_UID, nickName),"owen:uid"));
}

int main() {
    ::testing::InitGoogleTest();
    auto ret = RUN_ALL_TESTS();
    getchar();
}

