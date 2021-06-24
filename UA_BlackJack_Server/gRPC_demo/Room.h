#pragma once

#include <vector>
#include <unordered_set>
#include "Players.h"

namespace ua_black_jack_server { namespace lobby{

/*
    room doesn't need to save the status of itself, thus the lobby directly change the status of players, not through the room!
*/
class Room{
public:
    using UID = int64_t;
    
public:
    void Match_end();

    void Join_room(UID uid);

    void Leave_room(UID uid);

    void Ready(UID uid);

    bool isDone();

    bool isFull();

    bool isEmpty();
    
    int unReadySize();

    //return all players of ready and unready in this room
    std::vector<UID> getAllPlayersID();
    //void Start();//no need in class room

private:
    std::unordered_set<UID> players_ready;
    std::unordered_set<UID> players_unready;
};
}
}
