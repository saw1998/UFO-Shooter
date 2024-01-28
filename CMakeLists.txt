cmake_minimum_required(VERSION 3.27)
project(UFO-Shooter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES src/main.cpp)
add_executable(UFO-Shooter src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Vec2.cpp
        src/Vec2.h
        src/Entity.cpp
        src/Entity.h
        src/Components.h
        src/EntityManager.cpp
        src/EntityManager.h
        src/Config.h)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(UFO-Shooter sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
