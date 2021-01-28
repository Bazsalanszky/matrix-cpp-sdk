cmake_minimum_required(VERSION 3.17)
project(matrix-cpp-sdk)

set(CMAKE_CXX_STANDARD 14)



add_subdirectory(external/curl)
add_subdirectory(external/jsoncpp)
set(SOURCE_FILES src/WebAPI.h src/WebAPI.cpp src/Client.h src/Client.cpp main.cpp src/Homeserver.cpp src/Homeserver.h src/Room.cpp src/Room.h src/Event.cpp src/Event.h src/RoomMember.cpp src/RoomMember.h)
add_executable(matrix-cpp-bot ${SOURCE_FILES})
target_link_libraries(matrix-cpp-bot libcurl)
target_link_libraries(matrix-cpp-bot jsoncpp_lib)
include_directories(${CURL_INCLUDE_DIR})
include_directories(${Jsoncpp_INCLUDE_DIR})
