#############################################################################
# Name:       CMakeLists.txt
# Purpose:    Unit test runner for wxSimpleJSON
# Author:     Blake Madden
# Created:    2025-09-03
# Copyright:  (c) 2025 Blake Madden
# License:    3-Clause BSD license
#############################################################################

cmake_minimum_required(VERSION 3.25)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(simplejsontest)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include(FindGit.cmake)
if(NOT Git_FOUND)
  message(FATAL_ERROR "Git not found. Unable to fetch needed libraries.")
endif()

include(FetchContent)
set(wxBUILD_SHARED OFF)
message(STATUS "Fetching wxWidgets...")
FetchContent_Declare(
  wxWidgets
  GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
  GIT_TAG        v3.3.1 # latest dev branch
)
message(STATUS "Fetching GoogleTest...")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(wxWidgets googletest)

add_executable(${CMAKE_PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/../src/cJSON/cJSON.c
  ${PROJECT_SOURCE_DIR}/../src/wxSimpleJSON.cpp
  ${PROJECT_SOURCE_DIR}/testmainc.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GTest::gtest_main wx::core wx::base)
include(CTest)
include(GoogleTest)
gtest_discover_tests(${CMAKE_PROJECT_NAME})