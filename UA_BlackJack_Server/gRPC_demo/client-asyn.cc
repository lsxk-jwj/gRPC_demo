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

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

// Service dependent
/*************************/
using demo::GetNameService;
using demo::NameReply;
using demo::NameRequest;
/*************************/

class Client {
 public:
  explicit Client(std::shared_ptr<Channel> channel)
      : stub_(GetNameService::NewStub(channel)) {}

  // Assembles the client's payload and sends it to the server.
  void GetName(int id) {
    // Data we are sending to the server.
    NameRequest request;
    request.set_id(id);

    // Call object to store rpc data
    AsyncClientCall* call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncGetName(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void*)call);
  }

  // Loop while listening for completed responses.
  // Prints out the response from the server.
  void AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (cq_.Next(&got_tag, &ok)) {
      AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

      GPR_ASSERT(ok);

      if (call->status.ok())
        std::cout << "Greeter received: " << call->reply.name() << std::endl;
      else
        std::cout << "RPC failed" << std::endl;

      delete call;
    }
  }

 private:
  // struct for keeping state and data information
  struct AsyncClientCall {
    NameReply reply;

    ClientContext context;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<NameReply>> response_reader;
  };

  std::unique_ptr<GetNameService::Stub> stub_;
  CompletionQueue cq_;
};

int main(int argc, char** argv) {
  Client client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  // Spawn reader thread that loops indefinitely
  std::thread thread_ = std::thread(&Client::AsyncCompleteRpc, &client);

  for (int i = 0; i < 10; i++) {
    int id = i;
    client.GetName(id);  // The actual RPC call!
  }

  std::cout << "Press control-c to quit" << std::endl << std::endl;
  thread_.join();  // blocks forever

  return 0;
}
