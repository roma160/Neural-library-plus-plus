cmake_minimum_required(VERSION 3.15)
project(NeuralNetworkTry)
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCE_FILES "src/*.tpp" "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(
       ${PROJECT_NAME} PRIVATE "./src"
)

set("TEST" "${PROJECT_NAME}_TEST")
add_executable(${TEST} "./testing/main.cpp")
target_link_libraries(${TEST} PRIVATE ${PROJECT_NAME})
target_include_directories(
       ${TEST} PRIVATE "./src"
)