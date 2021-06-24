#include "Players.h"
#include <cassert>

constexpr int kPlayingFlag = 100000007;

ua_black_jack_server::lobby::Players::Status ua_black_jack_server::lobby::Players::GetStatus(const UID uid) const {
    if (!players_.count(uid))
        return Status::OFFLINE;
    const auto status = players_.at(uid);
    if (status < 0) {
        return Status::IN_ROOM_NOT_READY;
    }
    if (status > kPlayingFlag) {
        return Status::PLAYING;
    }
    if (status == 0) {
        return Status::IN_LOBBY;
    }
    return Status::IN_ROOM_READY;
}

ua_black_jack_server::lobby::Players::RoomID ua_black_jack_server::lobby::Players::getRoom(const UID uid) const {
    const auto status = GetStatus(uid);
    if (status == Status::OFFLINE)
        return -1;
    const auto rid = players_.at(uid);
    if (rid < 0) {
        return -rid;
    }
    if (rid == 0)
        return 0;
    if (rid > kPlayingFlag) {
        return rid - kPlayingFlag;
    }
    return rid;
}

void ua_black_jack_server::lobby::Players::NewPlayer(const UID uid) {
    // return false
    assert(players_.find(uid) == players_.end());
    players_[uid] = 0;// indicates in Lobby now
    // return true;
}

void ua_black_jack_server::lobby::Players::JoinRoom(const UID uid, const RoomID rid) {
    assert(players_[uid] == 0);
    players_[uid] = -rid;
}

void ua_black_jack_server::lobby::Players::LeaveRoom(const UID uid) {
    assert(players_.count(uid));
    players_[uid] = 0;
}

void ua_black_jack_server::lobby::Players::LogOut(const UID uid) {
    assert(players_.count(uid));
    players_.erase(uid);
}

void ua_black_jack_server::lobby::Players::MatchStart(const UID uid) {
    assert(players_[uid] > 0 && players_[uid] < kPlayingFlag);
    players_[uid] += kPlayingFlag;
}

void ua_black_jack_server::lobby::Players::MatchEnd(const UID uid) {
    assert(players_[uid] > kPlayingFlag);
    const auto room_id = players_[uid] - kPlayingFlag;
    players_[uid]      = -room_id;
}

void ua_black_jack_server::lobby::Players::Ready(const UID uid) {
    assert(players_[uid] < 0);
    players_[uid] = -players_[uid];
}

std::string ua_black_jack_server::lobby::Players::StateToString(const Status status) {
    switch (status) {
    case Status::IN_LOBBY:
        return "IN_LOBBY";
    case Status::IN_ROOM_READY:
        return "IN_ROOM_READY";
    case Status::IN_ROOM_NOT_READY:
        return "IN_ROOM_NOT_READY";
    case Status::PLAYING:
        return "PLAYING";
    case Status::OFFLINE:
        return "OFFLINE";
    default: // NOLINT(clang-diagnostic-covered-switch-default)
        return "UNKNOWN";
    }
}
