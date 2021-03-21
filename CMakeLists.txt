cmake_minimum_required(VERSION 3.15)
project(NeuralNetworkTry)
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(
        ${PROJECT_NAME} "src"
)