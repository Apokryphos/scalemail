cmake_minimum_required (VERSION 2.9)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Werror")

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

project(scalemail)

set(SCALEMAIL_INCLUDE
    include/engine.hpp)

set(SCALEMAIL_SOURCE
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SCALEMAIL_SOURCE} ${SCALEMAIL_SOURCE})
include_directories(include)
target_link_libraries(${PROJECT_NAME} scalemail_engine)