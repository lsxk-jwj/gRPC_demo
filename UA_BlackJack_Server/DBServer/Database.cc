#include "Database.h"

using namespace ua_black_jack_server::data_base_server;

int64_t Database::SignUp(const std::string& nickname, const std::string& password) {
    if (service_.NameExists(nickname.c_str()))
        return -1;
    const auto uid = service_.NextUid();
    service_.SetNickname(uid, nickname.c_str());
    service_.setPassword(uid, password.c_str());
    service_.SetUid(nickname.c_str(), uid);
    service_.SetScore(uid, init_score_);

    service_.UpdateRank(uid, init_score_);
    return uid;
}

std::string Database::GetPassword(int64_t uid) {
    return service_.GetPassword(uid).c_str();
}

std::string Database::GetNickname(int64_t uid) {
    return service_.GetNickname(uid).c_str();
}

int64_t Database::GetUid(const std::string& nickname) {
    const std::string uid = service_.GetUid(nickname.c_str()).c_str();
    return std::stoll(uid);
}

int Database::GetScore(int64_t uid) {
    return service_.GetScore(uid);
}

static std::vector<std::string> ToStdStringVec(const std::vector<acl::string>& aclVec) {
    std::vector<std::string> result;
    result.reserve(aclVec.size());
    for (const auto& str : aclVec) {
        result.emplace_back(str.c_str());
    }
    return result;
}

std::vector<std::string> Database::GetFriendList(int64_t uid) {
    return ToStdStringVec(service_.GetFriendList(uid));
}

std::vector<std::string> Database::GetWaitingFriendList(int64_t uid) {
    return ToStdStringVec(service_.GetWaitingFriendList(uid));
}

std::vector<std::string> Database::getMatchList(int64_t uid) {
    return ToStdStringVec(service_.GetMatchList(uid));
}

int Database::RankMe(int64_t uid) {
    return service_.GetRank(uid);
}

std::vector<std::string> Database::RankTop(int index) {
    return ToStdStringVec(service_.GetTopPlayer(index));
}

bool Database::AddFriend(int64_t uid, int64_t friendID) {
    return service_.InsertFriendList(uid, friendID);
}

bool Database::DeleteFriend(int64_t uid, int64_t friendID) {
    return service_.RemoveFriendList(uid, friendID);
}

bool Database::AddWaitFriend(int64_t uid, int64_t friendID) {
    return service_.InsertFriendList(uid, friendID);
}

bool Database::DeleteWaitingFriend(int64_t uid, int64_t friendID) {
    return service_.RemoveFriendList(uid, friendID);
}

bool Database::MatchFinish(const std::map<std::string, std::string>& match_info) {
    const auto match_id = service_.NextMatchId();
    // service_.InsertMatchInfo(match_id, match_info);
    for (const auto& p : match_info) {
        if (p.first == "time") {
            // this is match info
            service_.InsertMatchInfo(match_id, p.first.c_str(), p.second.c_str());
        }
        else {
            // for each player, add score and add match list
            const auto uid  = std::stoll(p.first);
            const auto diff = std::stoi(p.second);
            service_.AddScore(uid, diff);
            service_.InsertMatchList(uid, match_id);
            service_.AddRankScore(uid, diff);
            service_.InsertMatchInfo(match_id, p.first.c_str(), p.second.c_str());
        }
    }
    return true;
}

std::map<std::string, std::string> Database::GetMatchInfo(int64_t mid) {
    auto ret = service_.GetMatchInfo(mid);

    std::map<std::string, std::string> result;
    for (const auto& p : ret) {
        result.emplace(p.first.c_str(), p.second.c_str());
    }
    return result;
}
