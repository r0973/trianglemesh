cmake_minimum_required(VERSION 3.11)

project(trianglemesh)

message(STATUS "triaglemesh: cmake_project_source_dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "trinaglemesh: cmake_current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

set(
	TRIANGLEMESH_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/trianglemesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/detail/trianglemesh.cpp
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

message("cmake_build_type: " ${CMAKE_BUILD_TYPE})

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++14 -Wall -Wno-deprecated-declarations") # -Wextra -pedantic -Wold-style-cast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fvisibility=hidden")
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(CMAKE_CXX_FLAGS_DEBUG "-g -D _DEBUG")
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")  
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRIANGLEMESH_EXPORTS")  

message(STATUS "cmake_cxx_flags: " ${CMAKE_CXX_FLAGS})

#add_subdirectory(triangle)

set(TARGET_LIB_NAME ${PROJECT_NAME})

add_library(
	${TARGET_LIB_NAME}
	SHARED
	${TRIANGLEMESH_SOURCES}
)

add_library(sub::trianglemesh ALIAS ${PROJECT_NAME})

target_link_libraries(
	${TARGET_LIB_NAME} 
	sub::triangle
)

target_include_directories(
	${PROJECT_NAME}	
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
)

#add_subdirectory(test)
