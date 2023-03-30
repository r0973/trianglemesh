cmake_minimum_required(VERSION 3.11)

project(test LANGUAGES CXX)

set(
	TEST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

message("test project_source_dir: ${PROJECT_SOURCE_DIR}")
message("test cmake_current_source_dir: ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(
	${PROJECT_NAME}
	${TEST_SOURCES}
)

target_link_libraries(
	${PROJECT_NAME} 
	sub::trianglemesh
)

target_include_directories(
	${PROJECT_NAME}	
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
)
