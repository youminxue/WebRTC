cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
PROJECT(sioclient)

if(NOT CMAKE_BUILD_TYPE )
MESSAGE(STATUS "not define build type, set to release" )
set(CMAKE_BUILD_TYPE Release )
elseif(NOT (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug" ))
MESSAGE(SEND_ERROR "CMAKE_BUILD_TYPE must be either Release or Debug")
return()
endif()

set(BOOST_VER "1.55.0" CACHE STRING "boost version" )
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost ${BOOST_VER} REQUIRED COMPONENTS system date_time random)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/socket.io ALL_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/socket.io/internal ALL_SRC)

add_library(sioclient ${ALL_SRC})
target_include_directories(sioclient PRIVATE ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/socket.io
)

set_property(TARGET sioclient PROPERTY CXX_STANDARD 11)
set_property(TARGET sioclient PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(sioclient PRIVATE ${Boost_LIBRARIES})
