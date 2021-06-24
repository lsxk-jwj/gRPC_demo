#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


#include "Player.grpc.pb.h"
#include "RedisService.h"

using ::grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace ua_black_jack_server::data_base_server {
std::shared_ptr<RedisService> server = nullptr;

//这是个普通函数
auto SignUp(const std::string nickname, std::string password) -> int64_t {
    if (server->NameExists(nickname.c_str()))
        return -1;
    auto uid = server->NextUid();
    server->SetNickname(uid, nickname.c_str());
    server->setPassword(uid, password.c_str());
    server->SetUid(nickname.c_str(), uid);
    return uid;
}
};

class DBServiceImpl : public player::DatabaseService::Service {

    //真正实现的RequestDB。DBservice里只有这一个接口供rpc调用
    ::grpc::Status RequestDB(::grpc::ServerContext* context, const ::player::Request* request,
                             ::player::Response*    response) override {
        std::cout << "new request come from " << context->peer() << std::endl;


        if (const auto type = request->requesttype(); type == player::Request_RequestType_SIGNUP) {
            const auto& args = request->args();
            if (const auto size = request->args_size(); size != 2) {
                response->set_status(-1);
                return Status::OK;
            }
            const auto& nickname = args.Get(0);
            const auto& password = args.Get(1);

            const auto uid = ua_black_jack_server::data_base_server::SignUp(nickname, password);
            response->set_uid(uid);
            return Status::OK;
        }
        response->set_status(-1);
        return Status::CANCELLED;
    }
};

void RunServer() {
    std::string   server_address("0.0.0.0:50051");
    DBServiceImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main() {

    // acl::string
    acl::redis_client client("127.0.0.1:6379");
    {
        acl::redis_string conn(&client);
        auto              ret = conn.incr("UID");
        if (ret) {
            std::cout << "redis connection success" << std::endl;
        }
    }

    //初始化RedisService* server
    ::ua_black_jack_server::data_base_server::server = std::make_shared<
        ua_black_jack_server::data_base_server::RedisService>(client);

    auto uid = ::ua_black_jack_server::data_base_server::server->NextUid();
    std::cout << "current uid is " << uid <<std::endl;
    RunServer();
}
