cmake_minimum_required(VERSION 3.15)
#TODO: rename all names to lowercase to make CLI port
project(NeuralNetworkTry)
set(CMAKE_CXX_STANDARD 14)

file("GLOB_RECURSE" "SOURCE_FILES" "src/*.tpp" "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

include_directories("./src")
link_libraries(${PROJECT_NAME})

set("TEST" "${PROJECT_NAME}_TEST")
add_executable(${TEST} "./testing/main.cpp")

set("NPY" "npy_to_traindata")
add_executable(${NPY} "./utils/npy_to_traindata/main.cpp")