#include "../RedisService.h"

#include <gtest/gtest.h>
#include <unordered_set>
using ua_black_jack_server::data_base_server::RedisService;

RedisService* service;

const char* nickname = "owen1";
const int64_t uid = 2345;
const char* password = "ASIKrgubhy";

int main() {
    acl::redis_client client("127.0.0.1:6379");
    acl::redis        conn(&client);
    conn.set("UID", "2345");
    conn.set("MID", "1234");
    service = new RedisService(client);

    ::testing::InitGoogleTest();
    auto ret = RUN_ALL_TESTS();


    getchar();

    delete service;
    client.close();
}

TEST(RedisService, nicknameToUID) {
    // const char*       nickname = "owen1";
    // constexpr int64_t uid      = 2345;
    EXPECT_TRUE(service->SetUid(nickname, uid));
    EXPECT_TRUE(service->NameExists(nickname));
    EXPECT_EQ(service->GetUid(nickname), acl::string("2345"));
}

TEST(RedisService, nextUID) {
    EXPECT_EQ(service->NextUid(), 2346);
    EXPECT_EQ(service->NextUid(), 2347);
    EXPECT_EQ(service->NextUid(), 2348);
    EXPECT_EQ(service->NextUid(), 2349);
    EXPECT_EQ(service->NextUid(), 2350);
}

TEST(RedisService, nextMatchId) {
    EXPECT_EQ(service->NextMatchId(), 1235);
    EXPECT_EQ(service->NextMatchId(), 1236);
    EXPECT_EQ(service->NextMatchId(), 1237);
    EXPECT_EQ(service->NextMatchId(), 1238);
    EXPECT_EQ(service->NextMatchId(), 1239);
    EXPECT_EQ(service->NextMatchId(), 1240);
}

TEST(RedisService, UIDToPassword) {
    // ;
    // const int64_t uid      = 2234;
    EXPECT_TRUE(service->setPassword(uid, password));
    EXPECT_EQ(service->GetPassword(uid), acl::string(password));
}

TEST(RedisService, UIDToNickname) {
    // const char*   nickname = "owen1";
    // const int64_t uid      = 2345;
    EXPECT_TRUE(service->SetNickname(uid, nickname));
    EXPECT_EQ(service->GetNickname(uid), acl::string(nickname));
}

TEST(RedisService, UIDToScore) {
    EXPECT_TRUE(service->SetScore(uid, 1000));
    EXPECT_EQ(service->GetScore(uid), 1000);
    EXPECT_TRUE(service->AddScore(uid, 1000));
    EXPECT_EQ(service->GetScore(uid), 2000);
    EXPECT_TRUE(service->AddScore(uid, -1000));
    EXPECT_EQ(service->GetScore(uid), 1000);
}

TEST(RedisService, UIDToFriendList) {
    EXPECT_TRUE(service->InsertFriendList(uid, 1234));
    EXPECT_TRUE(service->InsertFriendList(uid, 1235));
    EXPECT_TRUE(service->InsertFriendList(uid, 1236));
    EXPECT_TRUE(service->InsertFriendList(uid, 1237));

    auto ret = service->GetFriendList(uid);
    EXPECT_EQ(ret.size(), 4);
    std::unordered_set<std::string> set1;
    set1.insert("1234");
    set1.insert("1235");
    set1.insert("1236");
    set1.insert("1237");
    for(const auto str : ret) {
        set1.erase(std::string(str.c_str()));
    }
    EXPECT_TRUE(set1.empty());

    EXPECT_TRUE(service->RemoveFriendList(uid, 1234));
    EXPECT_EQ(service->GetFriendList(uid).size(), 3);
}


TEST(RedisService, UIDToRank) {
    EXPECT_TRUE(service->UpdateRank(uid, 1000));
    EXPECT_TRUE(service->UpdateRank(uid + 1, 2000));
    EXPECT_TRUE(service->UpdateRank(uid + 2, 3000));
    EXPECT_EQ(service->GetRank(uid), 3);
    EXPECT_EQ(service->GetRank(uid + 1), 2);
    EXPECT_EQ(service->GetRank(uid + 2), 1);

    EXPECT_TRUE(service->AddRankScore(uid, 3000));
    EXPECT_EQ(service->GetRank(uid), 1);

    auto ret = service->GetTopPlayer(2);
    EXPECT_EQ(ret.size(), 2);
    decltype(ret) rank = { "2345", "2347" };
    EXPECT_EQ(rank, ret);
}

TEST(RedisService, MatchList) {
    EXPECT_TRUE(service->InsertMatchList(uid, 1000));
    EXPECT_TRUE(service->InsertMatchList(uid, 1200));
    EXPECT_TRUE(service->InsertMatchList(uid, 1300));
    EXPECT_TRUE(service->InsertMatchList(uid, 1400));
    const std::vector<acl::string> vec = {"1400", "1300", "1200", "1000"};
    EXPECT_EQ(service->GetMatchList(uid), vec);
}

TEST(RedisService, MatchInfo) {
    EXPECT_TRUE(service->InsertMatchInfo(1000, "time", "12000"));
    EXPECT_TRUE(service->InsertMatchInfo(1000, "2345", "+12"));
    EXPECT_TRUE(service->InsertMatchInfo(1000, "2346", "-24"));
    EXPECT_TRUE(service->InsertMatchInfo(1000, "2347", "+12"));
    EXPECT_EQ(service->GetMatchInfo(1000).size(), 4);
}
