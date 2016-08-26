cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

option(HAUSBUS_CMAKE_DEBUG "Set to on for some Debug output" Off)

set(HAUSBUS_VERSION_MAJOR 0)
set(HAUSBUS_VERSION_MINOR 1)
set(HAUSBUS_VERSION_PATCH 0)

set(_HAUSBUS_VERSION
        "${HAUSBUS_VERSION_MAJOR}.${HAUSBUS_VERSION_MINOR}.${HAUSBUS_VERSION_PATCH}")

mark_as_advanced(_HAUSBUS_VERSION)

set(PROJECT_NAME hausbus)

# CMP0048 check if policy is present
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
    project(${PROJECT_NAME} VERSION ${_HAUSBUS_VERSION} LANGUAGES CXX)
else()
    project(${PROJECT_NAME} CXX)
endif()


if(HAUSBUS_CMAKE_DEBUG)
    message(STATUS "[${CMAKE_CURRENT_LIST_DIR}:${CMAKE_CURRENT_LIST_LINE}] "
            "${PROJECT_NAME} version: ${_HAUSBUS_VERSION}")
endif()

add_definitions(-std=c++1y)

# ====================== build stuff ===========================================

set(_HAUSBUS_HDRS
        include/crc8.h
        include/data.h
        include/hausbus.h
        include/moodlights.h)

set(_HAUSBUS_SRCS
        src/crc8.cpp
        src/hausbus.cpp
        src/moodlights.cpp)

add_library(${PROJECT_NAME} ${_HAUSBUS_SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${_HAUSBUS_HDRS}")
target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

mark_as_advanced(
        _HAUSBUS_HDRS
        _HAUSBUS_SRCS)

# ====================== config stuff ==========================================
# Version
configure_file(hausbusConfigVersion.cmake.in
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Config
configure_file(hausbusConfig.cmake.in
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)


# ====================== install stuff =========================================
set(_INSTALL_CMAKE_DIR lib/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})

# Install config stuff
install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${_INSTALL_CMAKE_DIR})
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${_INSTALL_CMAKE_DIR}" COMPONENT dev)

mark_as_advanced(_INSTALL_CMAKE_DIR)
