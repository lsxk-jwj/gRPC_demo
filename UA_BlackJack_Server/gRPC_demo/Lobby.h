#pragma once

#include <stdio.h>
#include <unordered_set>
#include <list>
#include <vector>
#include <cassert>
#include <algorithm>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Players.h"
#include "Room.h"

constexpr int maxRooms = 20000;

namespace ua_black_jack_server { 
namespace lobby {

class Lobby{
public:
    using UID = int64_t;
    using RoomID = int32_t;

public:

    Lobby();

    UID Login(std::string nickname, std::string password);

    void Logout(UID uid);

    bool JoinRoom(UID uid, RoomID rid);

    RoomID CreateRoom(UID uid);

    bool LeaveRoom(UID uid);

    bool PlayerReady(UID uid);

    RoomID QuickMatch(UID uid);

    std::vector<RoomID> GetAvailableRoomList();

    void MatchEnd(RoomID rid);

    //for debug
    int GetPlayersSizeInRoom(RoomID rid);
    enum class RoomStatus{
        EMPTY,
        AVAILABLE,
        OCCUPIED,
        FULL,
        NOT_EXIST
    };
    RoomStatus GetRoomStatus(RoomID rid);

private:
    bool CheckRoomDone(RoomID rid);
    
    //RPC call, send data to match service
    bool MatchStart(RoomID rid, std::vector<UID> playersID);

private:
    //the status of player is managed by Players.
    Players AllPlayers_;
    //the status of room is managed by these four set in order to exceed the search.
    std::unordered_set<RoomID> emptyRooms_;
    std::unordered_set<RoomID> availableRooms_;//still can be added in.
    std::unordered_set<RoomID> occupiedRooms_;//rooms for match already started. it can be transferred from either avaiableRooms_ or fullRooms_.
    std::unordered_set<RoomID> fullRooms_;

    std::vector<Room> AllRooms_;
    RoomID curMaxRoomID;
    std::shared_ptr<spdlog::logger> logger;
};




}}
