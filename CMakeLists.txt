# this file also comiles the tests written in tests/ directory,
# supply external variable as "cmake -DBUILD_TESTS=ON .."
cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pubsub_server VERSION 1.0)

option(BUILD_TESTS "Build test programs" OFF)

# library for topics data structure
add_library(topics src/topics.cpp)
target_include_directories(topics PRIVATE include)

# library for messages data structure
add_library(messages src/messages.cpp)
target_include_directories(messages PRIVATE include)

# compiling test programs
if(BUILD_TESTS)
    # u test for topics
    add_executable(utest_topics tests/utest_topics.cpp)
    target_include_directories(utest_topics PRIVATE include)
    target_link_libraries(utest_topics topics)

    # u test for messages
    add_executable(utest_messages tests/utest_messages.cpp)
    target_include_directories(utest_messages PRIVATE include)
    target_link_libraries(utest_messages messages topics)
endif()
