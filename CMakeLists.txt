cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

option(LIBHAUSBUS_CMAKE_DEBUG "Set to on for some Debug output" Off)

set(LIBHAUSBUS_VERSION_MAJOR 1)
set(LIBHAUSBUS_VERSION_MINOR 0)
set(LIBHAUSBUS_VERSION_PATCH 0)

set(LIBHAUSBUS_VERSION "${LIBHAUSBUS_VERSION_MAJOR}.${LIBHAUSBUS_VERSION_MINOR}.${LIBHAUSBUS_VERSION_PATCH}")

project(libhausbus  VERSION ${LIBHAUSBUS_VERSION} LANGUAGES CXX)

if(LIBHAUSBUS_CMAKE_DEBUG)
    message(STATUS "[${CMAKE_CURRENT_LIST_DIR}:${CMAKE_CURRENT_LIST_LINE}] "
            "doorlockd version: ${LIBHAUSBUS_VERSION}")
endif()

add_definitions(-std=c++1y)

set(LIBHAUSBUS_HDRS
        include/crc8.h
        include/data.h
        include/hausbus.h
        include/moodlights.h)

set(LIBHAUSBUS_SRCS
        crc8.cpp
        hausbus.cpp
        moodlights.cpp)

add_library(hausbus STATIC ${LIBHAUSBUS_SRCS})

set_target_properties(hausbus PROPERTIES PUBLIC_HEADER "${LIBHAUSBUS_HDRS}")
target_include_directories(hausbus PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(TARGETS hausbus
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin
        PUBLIC_HEADER DESTINATION include/hausbus)
