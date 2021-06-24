cmake_minimum_required(VERSION 3.5.1)

project(Lobby C CXX)

include(common.cmake)

get_filename_component(hw_proto "lobby.proto" ABSOLUTE)  # demo dependent modified
get_filename_component(hw_proto_path "${hw_proto}" PATH)

set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/lobby.pb.cc") # demo dependent modified
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/lobby.pb.h")  # demo dependent modified
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/lobby.grpc.pb.cc") # demo dependent modified
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/lobby.grpc.pb.h")  # demo dependent modified
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# cpp file dependent: change server and client to your own cpp file (withou .cc/.cpp suffix)
foreach(_target
  server-syn client-syn)
  add_executable(${_target} "${_target}.cc" "Lobby.cpp" "Players.cpp" "Room.cpp") 
  target_link_libraries(${_target}
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

#添加
# link_directories( ${PROJECT_SOURCE_DIR})
# include_directories(${PROJECT_SOURCE_DIR}/include)

