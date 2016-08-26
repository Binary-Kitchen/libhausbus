cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

option(LIBHAUSBUS_CMAKE_DEBUG "Set to on for some Debug output" Off)

set(LIBHAUSBUS_VERSION_MAJOR 0)
set(LIBHAUSBUS_VERSION_MINOR 1)
set(LIBHAUSBUS_VERSION_PATCH 0)

set(LIBHAUSBUS_VERSION "${LIBHAUSBUS_VERSION_MAJOR}.${LIBHAUSBUS_VERSION_MINOR}.${LIBHAUSBUS_VERSION_PATCH}")

# CMP0048 check if policy is present
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
    project(libhausbus VERSION ${LIBHAUSBUS_VERSION} LANGUAGES CXX)
else()
    project(libhausbus CXX)
endif()


if(LIBHAUSBUS_CMAKE_DEBUG)
    message(STATUS "[${CMAKE_CURRENT_LIST_DIR}:${CMAKE_CURRENT_LIST_LINE}] "
            "libhausbus version: ${LIBHAUSBUS_VERSION}")
endif()

add_definitions(-std=c++1y)

# ====================== build stuff ===========================================

set(_LIBHAUSBUS_HDRS
        include/crc8.h
        include/data.h
        include/hausbus.h
        include/moodlights.h)

set(_LIBHAUSBUS_SRCS
        src/crc8.cpp
        src/hausbus.cpp
        src/moodlights.cpp)

add_library(hausbus ${_LIBHAUSBUS_SRCS})

set_target_properties(hausbus PROPERTIES PUBLIC_HEADER "${_LIBHAUSBUS_HDRS}")
target_include_directories(hausbus PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

mark_as_advanced(
        _LIBHAUSBUS_HDRS
        _LIBHAUSBUS_SRCS)

# ====================== install stuff =========================================

install(TARGETS hausbus
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin
        PUBLIC_HEADER DESTINATION include/hausbus)
