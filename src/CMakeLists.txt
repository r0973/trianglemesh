cmake_minimum_required(VERSION 3.11)

project(trianglemesh_test)

message(STATUS "triaglemesh: cmake_project_source_dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "trinaglemesh: cmake_current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(triangle)
add_subdirectory(trianglemesh)
add_subdirectory(test)

