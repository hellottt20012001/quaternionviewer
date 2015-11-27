cmake_minimum_required (VERSION 2.6)

project(QuaternionViewer)

# Include headers
include_directories(include)

# Sources
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
link_directories(${CMAKE_BINARY_DIR}/../lib)

# setup some flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -std=c++11 -Wall -W -pedantic -Wextra")

add_executable(QuaternionViewer ${SOURCES})

if(WIN32)
	#SET(CMAKE_C_COMPILER C:/MinGW/bin/gcc)
	#SET(CMAKE_CXX_COMPILER C:/MinGW/bin/g++)
	target_link_libraries(QuaternionViewer glfw3 glew32 opengl32)
else()
	target_link_libraries(QuaternionViewer glfw GLEW GL)
endif()
