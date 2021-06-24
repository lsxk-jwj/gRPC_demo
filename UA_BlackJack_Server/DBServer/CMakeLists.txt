cmake_minimum_required (VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
project(DBServer)

set(CXX_FLAGS -std=c++17)

# set(CMAKE_INCLUDE_PATH "/home/owen/code/grpc/")
include(common.cmake)

add_library(database RedisService.cc nameFormat.cc Database.cc)

# add_executable(demo RedisTest.cc )
# target_link_libraries(demo database _acl_cpp _protocol _acl pthread)

macro(add_test_target TEST_TARGET)
    add_executable("test_${TEST_TARGET}" "./test/test_${TEST_TARGET}.cc")
    target_link_libraries("test_${TEST_TARGET}" database _acl_cpp _protocol _acl gtest gtest_main pthread)
endmacro(add_test_target)

add_test_target(nameFormat)
add_test_target(RedisService)
add_test_target(Database)

add_executable(redisService "DBService.cc" "Player.grpc.pb.cc" "Player.pb.cc")
target_link_libraries(redisService ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} database _acl_cpp _protocol _acl pthread)

