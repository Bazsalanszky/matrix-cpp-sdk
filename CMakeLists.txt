cmake_minimum_required(VERSION 3.17)
project(matrix-cpp-sdk VERSION 1.0.1 DESCRIPTION "A very basic Matrix Client-Server SDK for C++")

set(CMAKE_CXX_STANDARD 14)

# Load submodules
add_subdirectory(external/curl)
add_subdirectory(external/jsoncpp)

include(GNUInstallDirs)

set(SOURCE_FILES src/WebAPI.cpp src/Client.cpp src/Homeserver.cpp src/Room.cpp src/Event.cpp src/RoomMember.cpp src/Logger.cpp include/matrix/EventListener.h src/EventListener.cpp src/Syncer.cpp src/Syncer.h)
add_library(matrix-cpp-sdk SHARED ${SOURCE_FILES})
target_link_libraries(matrix-cpp-sdk
        PRIVATE libcurl
        PRIVATE jsoncpp_lib
        )
target_include_directories(matrix-cpp-sdk
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
        PRIVATE src/
        PRIVATE include/matrix
        PRIVATE ${CURL_INCLUDE_DIR}
        PRIVATE ${Jsoncpp_INCLUDE_DIR}
        )

set(MATRIX_CPP_INCLUDE_DIR include/)

set_target_properties(matrix-cpp-sdk PROPERTIES PUBLIC_HEADER ${MATRIX_CPP_INCLUDE_DIR}/matrix/Client.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)

configure_file(matrix-cpp-sdk.pc.in matrix-cpp-sdk.pc @ONLY)
install(TARGETS matrix-cpp-sdk EXPORT matrix-cpp-sdk-cfg
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/matrix-cpp-sdk.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})