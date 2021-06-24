#include "Lobby.h"
#include <iostream>

using namespace ua_black_jack_server::lobby;
using std::cout;
using std::endl;
using std::cin;

int main(){
    Lobby lobbyImp;

    Lobby::UID a_uid = lobbyImp.Login("a", "000000");
    Lobby::UID b_uid = lobbyImp.Login("b", "000000");
    Lobby::UID c_uid = lobbyImp.Login("c", "000000");
    Lobby::UID d_uid = lobbyImp.Login("d", "000000");
    Lobby::UID e_uid = lobbyImp.Login("e", "000000");
    Lobby::UID f_uid = lobbyImp.Login("f", "000000");
    Lobby::UID g_uid = lobbyImp.Login("g", "000000");
    Lobby::UID h_uid = lobbyImp.Login("h", "000000");

    Lobby::RoomID firstRoomID = lobbyImp.CreateRoom(a_uid);
    Lobby::RoomID secondRoomID = lobbyImp.CreateRoom(a_uid);


    //for join room, leave room, roomstatus
    bool ret = lobbyImp.JoinRoom(a_uid, firstRoomID);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(b_uid, firstRoomID);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(c_uid, firstRoomID);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(d_uid, firstRoomID);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(e_uid, firstRoomID);
    assert(ret == true);
    auto status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    ret = lobbyImp.JoinRoom(f_uid, firstRoomID);
    assert(ret == true);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::FULL);

    lobbyImp.LeaveRoom(f_uid);
    lobbyImp.LeaveRoom(e_uid);

    lobbyImp.PlayerReady(a_uid);
    lobbyImp.PlayerReady(b_uid);
    lobbyImp.PlayerReady(c_uid);
    lobbyImp.PlayerReady(d_uid);

    ret = lobbyImp.PlayerReady(e_uid);
    assert(ret == false);
    
    //lobbyImp.PlayerReady(f_uid);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::OCCUPIED);
    lobbyImp.MatchEnd(firstRoomID);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    ret = lobbyImp.JoinRoom(e_uid, firstRoomID);
    ret = lobbyImp.JoinRoom(f_uid, firstRoomID);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::FULL);

    lobbyImp.PlayerReady(a_uid);
    lobbyImp.PlayerReady(b_uid);
    lobbyImp.PlayerReady(c_uid);
    lobbyImp.PlayerReady(d_uid);
    lobbyImp.PlayerReady(e_uid);
    lobbyImp.PlayerReady(f_uid);

    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::OCCUPIED);
    lobbyImp.MatchEnd(firstRoomID);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::FULL);

    ret = lobbyImp.LeaveRoom(a_uid);
    assert(ret == true);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    lobbyImp.PlayerReady(b_uid);
    lobbyImp.PlayerReady(c_uid);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    lobbyImp.LeaveRoom(d_uid);
    lobbyImp.LeaveRoom(e_uid);
    lobbyImp.LeaveRoom(f_uid);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::OCCUPIED);

    lobbyImp.MatchEnd(firstRoomID);


    /*
    ret = lobbyImp.JoinRoom(g_uid, firstRoomID);
    assert(ret == false);

    ret = lobbyImp.LeaveRoom(f_uid);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(g_uid, firstRoomID);
    assert(ret == true);
    ret = lobbyImp.JoinRoom(f_uid, firstRoomID);
    assert(ret == false);

    int n = lobbyImp.GetRoomSize(firstRoomID);
    assert(n == 6);
    */


    ret = lobbyImp.JoinRoom(g_uid, secondRoomID);
    assert(ret == true);
    Lobby::RoomID thirdRoomID = lobbyImp.CreateRoom(h_uid);
    assert(thirdRoomID != -1);
    lobbyImp.JoinRoom(h_uid, secondRoomID);
    lobbyImp.PlayerReady(h_uid);
    lobbyImp.PlayerReady(g_uid);
    status = lobbyImp.GetRoomStatus(secondRoomID);
    assert(status == Lobby::RoomStatus::OCCUPIED);
    int size = lobbyImp.GetPlayersSizeInRoom(secondRoomID);
    assert(size == 2);
    lobbyImp.MatchEnd(secondRoomID);

    lobbyImp.LeaveRoom(h_uid);
    lobbyImp.LeaveRoom(g_uid);
    auto newRoomID = lobbyImp.CreateRoom(h_uid);
    assert(newRoomID != -1);
    status = lobbyImp.GetRoomStatus(secondRoomID);
    assert(status == Lobby::RoomStatus::EMPTY);

    // b c in the first room not ready now.
    auto MatchRid = lobbyImp.QuickMatch(a_uid);
    assert(MatchRid == firstRoomID);
    MatchRid = lobbyImp.QuickMatch(d_uid);
    //MatchRid = lobbyImp.QuickMatch(e_uid);
    //MatchRid = lobbyImp.QuickMatch(f_uid);
    assert(MatchRid == firstRoomID);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    auto diffRid1 = lobbyImp.QuickMatch(g_uid);
    status = lobbyImp.GetRoomStatus(diffRid1);
    assert(status == Lobby::RoomStatus::AVAILABLE);

    auto diffRid2 = lobbyImp.QuickMatch(h_uid);
    status = lobbyImp.GetRoomStatus(diffRid2);
    assert(status == Lobby::RoomStatus::FULL);

    lobbyImp.PlayerReady(b_uid);
    lobbyImp.PlayerReady(c_uid);
    status = lobbyImp.GetRoomStatus(firstRoomID);
    assert(status == Lobby::RoomStatus::OCCUPIED);

    lobbyImp.GetAvailableRoomList();

    spdlog::info("a_uid: {0:d};  b_uid:  {1:d};  c_uid:  {2:d}", a_uid, b_uid, c_uid);
    spdlog::info("firstRoomID: {0: d}", firstRoomID);
    spdlog::info("secondRoomID: {0: d}", secondRoomID);
    return 0;
}