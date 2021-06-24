#pragma once


#include "RedisService.h"

namespace ua_black_jack_server :: data_base_server {

class Database {
public:
    Database(const std::string& redis_host, int init_score) :
        client_(redis_host.c_str()),
        service_(client_),
        init_score_(init_score) {
    }

    ~Database() {
        client_.close();
    }

    Database(const Database&)            = delete;
    Database(Database&&)                 = delete;
    Database& operator=(const Database&) = delete;
    Database& operator=(Database&&)      = delete;

    using MatchInfo = std::map<std::string, std::string>;

    int64_t     SignUp(const std::string& nickname, const std::string& password);
    std::string GetPassword(int64_t uid);

    std::string GetNickname(int64_t uid);

    int64_t GetUid(const std::string& nickname);

    int GetScore(int64_t uid);

    std::vector<std::string> GetFriendList(int64_t uid);
    std::vector<std::string> GetWaitingFriendList(int64_t uid);
    auto                     getMatchList(int64_t uid) -> std::vector<std::string>;

    int                      RankMe(int64_t uid);
    std::vector<std::string> RankTop(int index);

    bool AddFriend(int64_t uid, int64_t friendID);
    bool DeleteFriend(int64_t uid, int64_t friendID);

    bool AddWaitFriend(int64_t uid, int64_t friendID);
    bool DeleteWaitingFriend(int64_t uid, int64_t friendID);

    bool      MatchFinish(const MatchInfo& match_info);
    MatchInfo GetMatchInfo(int64_t mid);


private:
    acl::redis_client client_;
    RedisService      service_;
    const int         init_score_;
};

}
