cmake_minimum_required(VERSION 3.11)

project(triangle LANGUAGES C)

set(
	TRIANGLE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/triangle.c
	${CMAKE_CURRENT_SOURCE_DIR}/triangle.h
)

message("triangle: project_source_dir: " ${PROJECT_SOURCE_DIR})
message("triangle: cmake_current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

message("cmake_build_type: " ${CMAKE_BUILD_TYPE})
	
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTRILIBRARY")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DANSI_DECLARATORS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANSI_DECLARATORS")

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")  
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -fPIC -Wall -Wno-deprecated-declarations") # -Wextra -pedantic -Wold-style-cast")
	if(CMAKE_BUILD_TYPE MATCHES "Debug")	
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -D _DEBUG") 
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_TIMER -D_CRT_SECURE_NO_WARNINGS -DCPU86")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")  
endif()
	
message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})  
message("CMAKE_C_FLAGS  : " ${CMAKE_C_FLAGS})  

set(TARGET_LIB_NAME ${PROJECT_NAME})

add_library(
	${TARGET_LIB_NAME}
	OBJECT#SHARED
	${TRIANGLE_SOURCES}
)

add_library(sub::triangle ALIAS ${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}	
	INTERFACE#PUBLIC 
    ${PROJECT_SOURCE_DIR}#/triangle
)
