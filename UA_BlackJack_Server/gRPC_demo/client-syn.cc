/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <cassert>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
//#include "demo.grpc.pb.h"
#include "lobby.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Service dependent
/*************************/
using Lobby::LobbyService;
using Lobby::Request;
using Lobby::Response;
using Lobby::Request_RequestType;
/*************************/

using UID = int64_t;
using RoomID = int32_t;


class Client {
 public:
  Client(std::shared_ptr<Channel> channel)
      : stub_(LobbyService::NewStub(channel)) {
        ClientLogger = spdlog::stdout_color_mt("client Log");
        ClientLogger->set_level(spdlog::level::trace);

      }

  UID Login(std::string username, std::string password) {
    Request request;
    //request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_requesttype(Request_RequestType::Request_RequestType_LOGIN);
    request.set_stamp(time(0));
    request.add_args(username);
    request.add_args(password);

    Response reply;
    ClientContext context;

    Status status = stub_->Login(&context, request, &reply);
    UID uid = reply.uid();
    // client这边只判断reply.status 不判断uid成功与否
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("Login succeed ");
      }
      else{
        ClientLogger->warn("Login failed ");
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
    }
    return uid;
  }

  bool JoinRoom(UID uid, RoomID rid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_stamp(time(0));
    request.add_args(std::to_string(rid));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->JoinRoom(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("join room succeed ");
        return true;
      }
      else{
        ClientLogger->warn("join room failed ");
        return false;
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
      return false;
    }
  }

  RoomID CreateRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_CREATE_ROOM);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->CreateRoom(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1 && reply.args_size() == 1){
        ClientLogger->info("create room succeed");
        return std::stoi(reply.args(0));
      }
      else{
        ClientLogger->warn("create room failed  ");
        return -1;
      }

    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
      return -1;
    }
  }

  bool LeaveRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_LEAVE_ROOM);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->LeaveRoom(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("leave room succeed");
        return true;
      }
      else{
        ClientLogger->warn("leave room failed");
        return false;
      }

    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
      return false;
    }
  }

  bool PlayerReady(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_READY);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->PlayerReady(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("player ready succeed");
        return true;
      }
      else{
        ClientLogger->warn("player ready failed");
        return false;
      }

    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
      return false;
    }
  }

  RoomID QuickMatch(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_QUICK_MATCH);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->QuickMatch(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1 && reply.args_size() == 1){
        ClientLogger->info("quick match succeed");
        return std::stoi(reply.args(0));
      }
      else{
        ClientLogger->warn("quick match failed");
        return -1;
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
      return -1;
    }
  }

  std::vector<RoomID> GetAvailableRoomList() {
    Request request;
  
    request.set_requesttype(Request_RequestType::Request_RequestType_ROOM_LIST);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetAvailableRoomList(&context, request, &reply);
    // Act upon its status.
    std::vector<RoomID> roomlist;
    if (status.ok()) {
      if(reply.status() == 1 && reply.args_size()){
        for(int i = 0; i<reply.args_size(); i++){
          roomlist.push_back(std::stoi(reply.args(i)));
        }

        ClientLogger->info("get roomlist succeed");
      }
      else{
        ClientLogger->warn("get roomlist failed");
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");
    }
    return roomlist;
  }

  void MatchEnd(RoomID rid) {
    Request request;
    request.add_args(std::to_string(rid));
    request.set_requesttype(Request_RequestType::Request_RequestType_MATCH_END);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->MatchEnd(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("match end succeed");
      }
      else{
        ClientLogger->warn("match end failed");
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");

    }
  }

  void Logout(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_LOGOUT);
    request.set_stamp(time(0));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Logout(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      if(reply.status() == 1){
        ClientLogger->info("logout succeed");
      }
      else{
        ClientLogger->warn("logout failed");
      }
    } else {
      ClientLogger->warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      ClientLogger->warn( "rpc failed ");

    }
  }



 private:
  std::unique_ptr<LobbyService::Stub> stub_;
  std::shared_ptr<spdlog::logger> ClientLogger;
};

int main(int argc, char** argv) {
  std::string target_str = "localhost:50051";
  Client client(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())
  );


    UID a_uid = client.Login("a", "000000");
    UID b_uid = client.Login("b", "000000");
    UID c_uid = client.Login("c", "000000");
    UID d_uid = client.Login("d", "000000");
    UID e_uid = client.Login("e", "000000");
    UID f_uid = client.Login("f", "000000");
    UID g_uid = client.Login("g", "000000");
    UID h_uid = client.Login("h", "000000");

    RoomID firstRoomID = client.CreateRoom(a_uid);
    RoomID secondRoomID = client.CreateRoom(a_uid);
    //for join room, leave room, roomstatus
    bool ret = client.JoinRoom(a_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(b_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(c_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(d_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(e_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(f_uid, firstRoomID);
    assert(ret == true);
    client.LeaveRoom(f_uid);
    client.LeaveRoom(e_uid);
    client.PlayerReady(a_uid);
    client.PlayerReady(b_uid);
    client.PlayerReady(c_uid);
    client.PlayerReady(d_uid);
    ret = client.PlayerReady(e_uid);
    assert(ret == false);
    //client.PlayerReady(f_uid);
    client.MatchEnd(firstRoomID);
    ret = client.JoinRoom(e_uid, firstRoomID);
    ret = client.JoinRoom(f_uid, firstRoomID);
    client.PlayerReady(a_uid);
    client.PlayerReady(b_uid);
    client.PlayerReady(c_uid);
    client.PlayerReady(d_uid);
    client.PlayerReady(e_uid);
    client.PlayerReady(f_uid);

    client.MatchEnd(firstRoomID);
    ret = client.LeaveRoom(a_uid);
    assert(ret == true);
    client.PlayerReady(b_uid);
    client.PlayerReady(c_uid);
    client.LeaveRoom(d_uid);
    client.LeaveRoom(e_uid);
    client.LeaveRoom(f_uid);
    client.MatchEnd(firstRoomID);


    /*
    ret = client.JoinRoom(g_uid, firstRoomID);
    assert(ret == false);

    ret = client.LeaveRoom(f_uid);
    assert(ret == true);
    ret = client.JoinRoom(g_uid, firstRoomID);
    assert(ret == true);
    ret = client.JoinRoom(f_uid, firstRoomID);
    assert(ret == false);

    int n = client.GetRoomSize(firstRoomID);
    assert(n == 6);
    */
    ret = client.JoinRoom(g_uid, secondRoomID);
    assert(ret == true);
    RoomID thirdRoomID = client.CreateRoom(h_uid);
    assert(thirdRoomID != -1);
    client.JoinRoom(h_uid, secondRoomID);
    client.PlayerReady(h_uid);
    client.PlayerReady(g_uid);
    client.MatchEnd(secondRoomID);
    client.LeaveRoom(h_uid);
    client.LeaveRoom(g_uid);
    auto newRoomID = client.CreateRoom(h_uid);
    assert(newRoomID != -1);


    // b c in the first room not ready now.
    auto MatchRid = client.QuickMatch(a_uid);
    assert(MatchRid == firstRoomID);
    MatchRid = client.QuickMatch(d_uid);
    //MatchRid = client.QuickMatch(e_uid);
    //MatchRid = client.QuickMatch(f_uid);
    assert(MatchRid == firstRoomID);
    auto diffRid1 = client.QuickMatch(g_uid);
    auto diffRid2 = client.QuickMatch(h_uid);
    client.PlayerReady(b_uid);
    client.PlayerReady(c_uid);

    

    auto lists = client.GetAvailableRoomList();
    for(auto& rid:lists)
      std::cout<<rid <<' ';
    std::cout<<std::endl;

    return 0;
}
