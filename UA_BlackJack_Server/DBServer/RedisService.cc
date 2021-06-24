#include "RedisService.h"

#include <cstdlib>

using namespace ua_black_jack_server::data_base_server;


char buffer[64];

static const char* ToString(int64_t uid) {
    if constexpr (sizeof(int64_t) == sizeof(long)) {
        snprintf(buffer, sizeof buffer, "%ld", uid);
    }
    else {
        snprintf(buffer, sizeof buffer, "%lld", uid); // NOLINT(clang-diagnostic-format)
    }
    return buffer;
}

bool RedisService::Exists(const char* key) {
    return conn_.exists(key);
}

acl::string RedisService::GetUid(const char* nickname) {
    const auto ret = conn_.get(GetKey(FormatType::NICKNAME_TO_UID, nickname));
    if (ret->get_type() == acl::REDIS_RESULT_NIL)
        return "";
    buffer_.clear();
    ret->argv_to_string(buffer_);
    return buffer_;
}

bool RedisService::NameExists(const char* nickname) {
    return Exists(GetKey(FormatType::NICKNAME_TO_UID, nickname));
}

bool RedisService::SetUid(const char* nickname, UID uid) {
    return conn_.set(GetKey(FormatType::NICKNAME_TO_UID, nickname), ToString(uid));
}

RedisService::String RedisService::GetPassword(UID uid) {
    const auto ret = conn_.get(GetKey(FormatType::UID_TO_PASSWORD, uid));
    if (ret->get_type() != acl::REDIS_RESULT_STRING)
        return "";
    buffer_.clear();
    ret->argv_to_string(buffer_);
    return buffer_;
}

bool RedisService::setPassword(UID uid, const char* password) {
    return conn_.set(GetKey(FormatType::UID_TO_PASSWORD, uid), password);
}


RedisService::String RedisService::GetNickname(UID uid) {
    const auto ret = conn_.get(GetKey(FormatType::UID_TO_NICKNAME, uid));
    if (ret->get_type() != acl::REDIS_RESULT_STRING)
        return "";
    buffer_.clear();
    ret->argv_to_string(buffer_);
    return buffer_;
}

bool RedisService::SetNickname(UID uid, const char* nickname) {
    return conn_.set(GetKey(FormatType::UID_TO_NICKNAME, uid), nickname);
}

int RedisService::GetScore(UID uid) {
    const auto ret = conn_.get(GetKey(FormatType::UID_TO_SCORE, uid));
    if (ret->get_type() != acl::REDIS_RESULT_STRING)
        return -1;
    ret->argv_to_string(buffer_);
    return atoi(buffer_.c_str()); // NOLINT(cert-err34-c)
}

bool RedisService::SetScore(UID uid, int score) {
    return conn_.set(GetKey(FormatType::UID_TO_SCORE, uid), ToString(score));
}

bool RedisService::AddScore(UID uid, int diff) {
    return conn_.incrby(GetKey(FormatType::UID_TO_SCORE, uid), diff);
}

RedisService::RepeatedString RedisService::GetFriendList(UID uid) {
    listBuffer_.clear();
    if (const auto ret = conn_.smembers(GetKey(FormatType::UID_TO_FRIEND_LIST, uid), &listBuffer_); ret <= 0) {
        return {};
    }
    return listBuffer_;
}

bool RedisService::InsertFriendList(UID uid, UID friendId) {
    return conn_.sadd(GetKey(FormatType::UID_TO_FRIEND_LIST, uid), ToString(friendId), nullptr);
}

bool RedisService::RemoveFriendList(UID uid, UID friendId) {
    return conn_.srem(GetKey(FormatType::UID_TO_FRIEND_LIST, uid), ToString(friendId), nullptr);
}

RedisService::RepeatedString RedisService::GetWaitingFriendList(UID uid) {
    listBuffer_.clear();
    if (const auto ret = conn_.smembers(GetKey(FormatType::UID_TO_WAIT_FRIEND_LIST, uid), &listBuffer_); ret <= 0) {
        return {};
    }
    return listBuffer_;
}

bool RedisService::InsertWaitingFriendList(UID uid, UID friendId) {
    return conn_.sadd(GetKey(FormatType::UID_TO_WAIT_FRIEND_LIST, uid), ToString(friendId), nullptr);

}

bool RedisService::RemoveWaitingFriendList(UID uid, UID friendId) {
    return conn_.srem(GetKey(FormatType::UID_TO_WAIT_FRIEND_LIST, uid), ToString(friendId), nullptr);
}

RedisService::RepeatedString RedisService::GetMatchList(UID uid) {
    listBuffer_.clear();
    if (conn_.lrange(GetKey(FormatType::UID_TO_MATCH_LIST, uid), 0, -1, &listBuffer_) <= 0)
        return {};
    return listBuffer_;
}

bool RedisService::InsertMatchList(UID uid, UID mid) {
    return conn_.lpush(GetKey(FormatType::UID_TO_MATCH_LIST, uid), ToString(mid), nullptr);
}

int RedisService::GetRank(UID uid) {
    return conn_.zrevrank(GetKey(FormatType::RANK), ToString(uid)) + 1;
}

bool RedisService::UpdateRank(UID uid, double score) {
    auto uidStr = ToString(uid);
    return conn_.zadd(GetKey(FormatType::RANK), &uidStr, &score, 1) == 1;
}

bool RedisService::AddRankScore(UID uid, int diff) {
    return conn_.zincrby(GetKey(FormatType::RANK), diff, ToString(uid));
}

RedisService::RepeatedString RedisService::GetTopPlayer(int index) {
    listBuffer_.clear();
    conn_.zrevrange(GetKey(FormatType::RANK), 0, index - 1, &listBuffer_);
    return listBuffer_;
}

RedisService::HashString RedisService::GetMatchInfo(UID matchId) {
    HashString result;
    conn_.hgetall(GetKey(FormatType::MATCH_ID_TO_MATCH_HISTORY, matchId), result);
    return result;
}

bool RedisService::InsertMatchInfo(UID matchId, const HashString& matchInfo) {
    return conn_.hmset(GetKey(FormatType::MATCH_ID_TO_MATCH_HISTORY, matchId), matchInfo);
}

bool RedisService::InsertMatchInfo(UID matchId, const char* key, const char* value) {
    return conn_.hset(GetKey(FormatType::MATCH_ID_TO_MATCH_HISTORY, matchId), key, value);
}

RedisService::UID RedisService::NextUid() {
    conn_.incr("UID");
    buffer_.clear();
    conn_.get("UID")->argv_to_string(buffer_);
    return strtoll(buffer_.c_str(), nullptr, 10);
}

int64_t RedisService::NextMatchId() {
    conn_.incr("MID");
    buffer_.clear();
    conn_.get("MID")->argv_to_string(buffer_);
    return strtoll(buffer_.c_str(), nullptr, 10);
}
