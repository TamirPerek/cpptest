cmake_minimum_required(VERSION 3.16)
project(cpptest VERSION 0.0.1 LANGUAGES CXX)

include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_DEBUG_POSTFIX _d)

include_directories(include)

set(HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpptest.hpp
)
set(MY_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpptest.cpp
)

add_library(${PROJECT_NAME}_Warnings INTERFACE)

# Add source files
add_library(
        ${PROJECT_NAME}
        SHARED
)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        ${MY_SOURCE_FILES}
        PUBLIC
        ${HEADER_FILES}
)

# Include directories
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME}_Warnings)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_Warnings)
