cmake_minimum_required(VERSION 3.5.0)
set (CMAKE_CXX_STANDARD 17)
project(game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include_directories(inc)
set(SourceFiles main.cpp src/jewels.cpp inc/gameBoard.inl inc/game.hpp inc/jewels.hpp inc/gameBoard.hpp src/game.cpp inc/common.hpp 
                inc/clock.hpp src/clock.cpp src/common.cpp assets/)

include_directories(
    "/Library/Frameworks/SDL2.framework/Versions/A/Headers"
    "/Library/Frameworks/SDL2_image.framework/Versions/A/Headers"
    "/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers")

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} 
                        "/Library/Frameworks/SDL2.framework/Versions/A/SDL2" 
                        "/Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image"
                        "/Library/Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf" )
target_include_directories(game PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")