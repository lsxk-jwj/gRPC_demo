#include "../Database.h"
#include <gtest/gtest.h>
using ua_black_jack_server::data_base_server::Database;

Database* database;

int64_t uids[4];

int main() {
    database = new Database("127.0.0.1:6379", 1000);
    ::testing::InitGoogleTest();
    auto ret = RUN_ALL_TESTS();
    getchar();
}

TEST(Database, MatchInfo) {

    uids[0] = database->SignUp("owen0", "password0");
    uids[1] = database->SignUp("owen1", "password1");
    auto ret = database->SignUp("owen0", "password1");
    EXPECT_EQ(ret, -1);
    uids[2] = database->SignUp("owen2", "password2");
    uids[3] = database->SignUp("owen3", "password3");
    EXPECT_EQ(uids[3] - 1, uids[2]);
    EXPECT_EQ(uids[1] - 1, uids[0]);

    EXPECT_EQ(database->GetPassword(uids[0]), "password0");
    EXPECT_EQ(database->GetNickname(uids[1]), "owen1");
    EXPECT_EQ(database->GetScore(uids[2]), 1000);
    EXPECT_EQ(database->GetUid("owen3"), uids[3]);
    EXPECT_EQ(database->GetFriendList(uids[0]).size(), 0);

    auto uid0 = std::to_string(uids[0]);
    auto uid1 = std::to_string(uids[1]);
    auto uid2 = std::to_string(uids[2]);
    auto uid3 = std::to_string(uids[3]);
    database->MatchFinish({{uid1, "+20"}, {uid2, "+10"}, {uid3, "-30"}});

    EXPECT_EQ(database->RankMe(uids[1]), 1);
    EXPECT_EQ(database->RankMe(uids[2]), 2);
    EXPECT_EQ(database->RankMe(uids[3]), 4);

    auto list = database->RankTop(4);
    EXPECT_EQ(list[0], uid1);
    EXPECT_EQ(list[1], uid2);
    EXPECT_EQ(list[3], uid3);

    EXPECT_EQ(database->GetScore(uids[1]), 1000 + 20);
    EXPECT_EQ(database->GetScore(uids[3]), 1000 -30);

    EXPECT_EQ(database->getMatchList(uids[0]).size(), 0);
    EXPECT_EQ(database->getMatchList(uids[1]).size(), 1);
}
