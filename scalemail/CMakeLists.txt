cmake_minimum_required(VERSION 3.10)

project(scalemail CXX)

set(CMAKE_BUILD_TYPE Debug)

set(SCALEMAIL_INCLUDE
    include/engine.hpp)

set(SCALEMAIL_SOURCE
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SCALEMAIL_SOURCE} ${SCALEMAIL_SOURCE})
include_directories(include)
target_link_libraries(${PROJECT_NAME} scalemail_engine)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wextra")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/Wall")
endif()