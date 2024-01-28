cmake_minimum_required(VERSION 3.27)
project(GameEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp)
add_executable(GameEngine main.cpp
        Game.cpp
        Game.h
        Vec2.cpp
        Vec2.h
        Entity.cpp
        Entity.h
        Components.h
        EntityManager.cpp
        EntityManager.h
        Config.h)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(GameEngine sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
