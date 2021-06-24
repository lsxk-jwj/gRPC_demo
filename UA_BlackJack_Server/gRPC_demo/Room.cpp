#include "Room.h"
#include <cassert>

void ua_black_jack_server::lobby::Room::Match_end(){
    std::swap(players_ready, players_unready);
}

void ua_black_jack_server::lobby::Room::Join_room(UID uid){
    assert(!isFull());
    players_unready.insert(uid);
}

void ua_black_jack_server::lobby::Room::Leave_room(UID uid){
    if(players_unready.count(uid))
        players_unready.erase(uid);
    if(players_ready.count(uid))
        players_ready.erase(uid);
}

void ua_black_jack_server::lobby::Room::Ready(UID uid){
    if(players_unready.find(uid)==players_unready.end() || players_ready.count(uid))
        return;
    players_unready.erase(uid);
    players_ready.insert(uid);
    
}

bool ua_black_jack_server::lobby::Room::isFull(){
    if((players_ready.size()+players_unready.size()) >= 6)
        return true;
    return false;
}

bool ua_black_jack_server::lobby::Room::isEmpty(){
    if(players_unready.size()==0 && players_ready.size()==0)
        return true;
    return false;
}

/*void ua_black_jack_server::lobby::Room::Start(){
    for(auto player_id : players_ready){
        Players::MatchStart(player_id);
    }
}
*/


bool ua_black_jack_server::lobby::Room::isDone(){
    if(players_ready.size()>=2 && players_unready.empty())
        return true;
    return false;

}

std::vector<ua_black_jack_server::lobby::Room::UID> ua_black_jack_server::lobby::Room::getAllPlayersID(){
    std::vector<UID> res;
    for(auto pUID:players_ready){
        res.push_back(pUID);
    }
    for(auto pUID:players_unready){
        res.push_back(pUID);
    }
    return res;

}

int ua_black_jack_server::lobby::Room::unReadySize(){
    return players_unready.size();
}