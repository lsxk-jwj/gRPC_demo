#include <iostream>
#include <memory>
#include <string>
#include "Lobby.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
//#include "demo.grpc.pb.h"
#include "lobby.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Service dependent
/*************************/
using Lobby::LobbyService;
using Lobby::Request;
using Lobby::Response;
using Lobby::Request_RequestType;
//using ua_black_jack_server::lobby::ServiceLogger;

/*************************/

namespace ua_black_jack_server { 
namespace lobby {
  std::shared_ptr<Lobby> lobbyImp = nullptr;
  //Lobby lobbyImp;
  std::shared_ptr<spdlog::logger> ServiceLogger = nullptr;

}
};

void StartLog(){
  ua_black_jack_server::lobby::ServiceLogger = spdlog::stdout_color_mt("Lobby Service Log");
  ua_black_jack_server::lobby::ServiceLogger->set_level(spdlog::level::trace);
}

class LobbyServiceImpl final : public LobbyService::Service {

  //Login先做成同步的！
  Status Login(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_LOGIN){

      auto args = request->args();
      if (request->args_size() != 2) {
        ua_black_jack_server::lobby::ServiceLogger->warn("the size of Request args is not available, request failed.");
        response->set_status(-1);
        return Status::OK;
      }
      auto username = args.Get(0);
      auto password = args.Get(1);
      auto uid = ua_black_jack_server::lobby::lobbyImp->Login(username, password);//done
      //uid == -1时和response.status == -1 都表示Login失败
      if(uid == -1){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }

      response->set_uid(uid);
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }


  //先实现这个简单的
  Status JoinRoom(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_JOIN_ROOM){

      auto args = request->args();
      auto uid = request->uid();
      if (request->args_size() != 1) {
        ua_black_jack_server::lobby::ServiceLogger->warn("the size of Request args is not available, request failed.");
        response->set_status(-1);
        return Status::OK;
      }
      auto rid = args.Get(0);
      auto ret = ua_black_jack_server::lobby::lobbyImp->JoinRoom(uid, std::stoi(rid));//done
      if(ret == false){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }

      //response->add_args(std::to_string(ret));大多数response不需要设置args
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status CreateRoom(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_CREATE_ROOM){

      auto uid = request->uid();
      
      auto rid = ua_black_jack_server::lobby::lobbyImp->CreateRoom(uid);//done
      if(rid == -1){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }

      response->add_args(std::to_string(rid));
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status LeaveRoom(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_LEAVE_ROOM){

      auto uid = request->uid();
      
      auto ret = ua_black_jack_server::lobby::lobbyImp->LeaveRoom(uid);//done
      if(ret == false){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }
      //response->add_args(std::to_string(ret));大多数response不需要设置args
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status PlayerReady(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_READY){

      auto uid = request->uid();
      
      auto ret = ua_black_jack_server::lobby::lobbyImp->PlayerReady(uid);//done
      if(ret == false){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }
      //response->add_args(std::to_string(ret));大多数response不需要设置args
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status QuickMatch(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_QUICK_MATCH){

      auto uid = request->uid();
      
      auto rid = ua_black_jack_server::lobby::lobbyImp->QuickMatch(uid);//done
      if(rid == -1){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }
      
      response->add_args(std::to_string(rid));
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status GetAvailableRoomList(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_ROOM_LIST){
      
      auto roomlist = ua_black_jack_server::lobby::lobbyImp->GetAvailableRoomList();
      if(roomlist.empty()){
        response->set_status(-1);
      }else{
        response->set_status(1);
      }
      
      for(auto& rid:roomlist)
        response->add_args(std::to_string(rid));
      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status MatchEnd(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_MATCH_END){

      auto args = request->args();
      if (request->args_size() != 1) {
        ua_black_jack_server::lobby::ServiceLogger->warn("the size of Request args is not available, request failed.");
        response->set_status(-1);
        return Status::OK;
      }
      auto rid = args.Get(0);
      ua_black_jack_server::lobby::lobbyImp->MatchEnd(std::stoi(rid));//done
      response->set_status(1);

      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }

  Status Logout(ServerContext* context, const Request* request,
                  Response* response) override {
    
    //always return Status::OK
    if(request->requesttype() == Request_RequestType::Request_RequestType_LOGOUT){
      auto uid = request->uid();
      ua_black_jack_server::lobby::lobbyImp->Logout(uid);//done
      response->set_status(1);

      return Status::OK;
    }
    else{
      ua_black_jack_server::lobby::ServiceLogger->warn("request from {}: requestType error", context->peer());
      response->set_status(-1);
      return Status::OK;
    }
  }



};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  LobbyServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {

  //ua_black_jack_server::lobby::lobbyImp = std::make_shared<ua_black_jack_server::lobby::Lobby> ();
  ua_black_jack_server::lobby::lobbyImp = std::make_shared<ua_black_jack_server::lobby::Lobby> ();
  StartLog();
  RunServer();

  return 0;
}