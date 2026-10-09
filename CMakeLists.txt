cmake_minimum_required(VERSION 3.20)

project(
    LyntraNet
    VERSION 0.5.1
    LANGUAGES CXX
)

# --------------------------------------------------
# Version
# --------------------------------------------------

set(LyntraNet_VERSION_TAG "dev")
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LT/Version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/LT/Version.h
    @ONLY
)

# --------------------------------------------------
# C++
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Sources
# --------------------------------------------------

file(GLOB_RECURSE LYNTRANET_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE LYNTRANET_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(LyntraNet STATIC)

target_sources(LyntraNet
    PRIVATE
        ${LYNTRANET_SOURCES}
        ${LYNTRANET_HEADERS}
)

# --------------------------------------------------
# Include
# --------------------------------------------------

target_include_directories(LyntraNet
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>

        $<INSTALL_INTERFACE:include>
)

# --------------------------------------------------
# Compile Options
# --------------------------------------------------

target_compile_features(LyntraNet PUBLIC cxx_std_20)

if(MSVC)

    target_compile_options(LyntraNet PRIVATE
        /permissive-
        /W4
        /MP
        /utf-8
        /Zc:__cplusplus
        /Zc:preprocessor
    )

    target_compile_definitions(LyntraNet PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

else()

    target_compile_options(LyntraNet PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()

# --------------------------------------------------
# Windows Libraries
# --------------------------------------------------

if(WIN32)

    target_link_libraries(LyntraNet PRIVATE
        ws2_32
        mswsock
    )

endif()

# --------------------------------------------------
# PCH
# --------------------------------------------------

target_precompile_headers(LyntraNet
    PRIVATE
        src/pch.h
)

# --------------------------------------------------
# Folder
# --------------------------------------------------

source_group(TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${LYNTRANET_SOURCES}
        ${LYNTRANET_HEADERS}
)

# --------------------------------------------------
# Tests
# --------------------------------------------------

enable_testing()
add_subdirectory(tests)