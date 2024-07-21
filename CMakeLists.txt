# this file also comiles the tests written in tests/ directory,
# supply external variable as "cmake -DBUILD_TESTS=ON .."
cmake_minimum_required(VERSION 3.29)

# global sets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

project(pubsub_server VERSION 1.0)

if(DEBUG_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(BUILD_TESTS "Build test programs" OFF)

# global include directories
include_directories(include)
include_directories(external/spdlog/include)
include_directories(external/Crow/include)
include_directories(include/boost)

# library for logger
add_library(logger src/logger.cpp)
if(DEBUG_BUILD)
    target_compile_definitions(logger PRIVATE DEBUG_BUILD)
endif()

# library for topics data structure
add_library(topics src/topics.cpp)
target_link_libraries(topics logger)

# library for messages data structure
add_library(messages src/messages.cpp)
target_link_libraries(messages logger)

# library for subscriptions data structure
add_library(subscriptions src/subscriptions.cpp)
target_link_libraries(subscriptions logger)

# main executable
add_executable(main src/main.cpp)
target_link_libraries(main logger topics messages subscriptions)
target_compile_definitions(main PRIVATE CROW_USE_BOOST)

# compiling test programs
if(BUILD_TESTS)
    # u test for topics
    add_executable(utest_topics tests/utest_topics.cpp)
    target_link_libraries(utest_topics topics)

    # u test for messages
    add_executable(utest_messages tests/utest_messages.cpp)
    target_link_libraries(utest_messages messages topics)

    # u test for subscriptions
    add_executable(utest_subscriptions tests/utest_subscriptions.cpp)
    target_link_libraries(utest_subscriptions subscriptions)
endif()
