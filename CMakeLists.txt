cmake_minimum_required(VERSION 3.27)
project(CVRP_optimization_metaheuristics)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INC_DIR})


set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")


file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)
