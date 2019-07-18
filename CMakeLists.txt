cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

project(simplejson)


#Generate the shared library from the library sources
add_library(simplejson SHARED 
    ${PROJECT_SOURCE_DIR}/src/cJSON.c
    ${PROJECT_SOURCE_DIR}/src/wxSimpleJSON.cpp
)

#Find and include wxWidgets
find_package(wxWidgets REQUIRED net core base)
include(${wxWidgets_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
target_link_libraries(simplejson ${wxWidgets_LIBRARIES})
