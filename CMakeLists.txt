cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

option(HAUSBUS_CMAKE_DEBUG "Set to on for some Debug output" Off)

set(PROJECT_NAME libhausbus)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_PATCH 0)

set(_${PROJECT_NAME}_VERSION
        "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

mark_as_advanced(_${PROJECT_NAME}_VERSION)

# CMP0048 check if policy is present
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
    project(${PROJECT_NAME} VERSION ${_${PROJECT_NAME}_VERSION} LANGUAGES CXX)
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

add_library(hausbus ${_HAUSBUS_SRCS})

target_include_directories(hausbus PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

mark_as_advanced(
        _HAUSBUS_HDRS
        _HAUSBUS_SRCS)

# ====================== config stuff ==========================================

include(${CMAKE_CURRENT_LIST_DIR}/vendor/CMakeTemplates/CMakeConfigTemplate.cmake)
add_cmake_config_files(${_${PROJECT_NAME}_VERSION} hausbus)

# ====================== install stuff =========================================
include(${CMAKE_CURRENT_LIST_DIR}/vendor/CMakeTemplates/CMakeInstallTemplate.cmake)
install_target(hausbus "${_HAUSBUS_HDRS}")
install_config_file()

# ====================== package stuff =========================================
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "hausbus lib")
set(CPACK_PACKAGE_VENDOR "Ralf Ramsauer")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/INSTALL.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${HAUSBUS_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${HAUSBUS_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${HAUSBUS_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

include(CPack)
