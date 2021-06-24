#pragma once

#include <unordered_map>
#include <string>


namespace ua_black_jack_server { namespace lobby {

class Players {
public:
    using UID = int64_t;
    using RoomID = int32_t;
    // using Status = int32_t;

    enum class Status {
        IN_LOBBY,
        IN_ROOM_NOT_READY,
        IN_ROOM_READY,
        PLAYING,
        OFFLINE,
    };

public:
    Status GetStatus(UID uid) const;

    /**
     * \brief get room id by player id
     * \param uid player id
     * \return roomId, except -1 means offline, 0 means in lobby
     */
    // ReSharper disable once CppInconsistentNaming
    RoomID getRoom(UID uid) const;

    void NewPlayer(UID uid);

    void JoinRoom(UID uid, RoomID rid);

    void LeaveRoom(UID uid);

    void LogOut(UID uid);

    void MatchStart(UID uid);

    void MatchEnd(UID uid);

    void Ready(UID uid);

    // for debug
    static std::string StateToString(Status status);

private:
    // players_ for saving status(also roomid) for each player
    std::unordered_map<UID, int32_t> players_;
};

}}
