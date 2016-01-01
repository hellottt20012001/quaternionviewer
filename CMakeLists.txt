cmake_minimum_required (VERSION 2.6)

project(QuaternionViewer)

# Include headers
include_directories(include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
link_directories(${CMAKE_BINARY_DIR}/..)

# setup some flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -std=c++11 -Wall -W -pedantic -Wextra -pthread")

# Sources
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(QuaternionViewer ${SOURCES})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system thread REQUIRED)

if(WIN32)
	target_link_libraries(QuaternionViewer ws2_32 wsock32 glfw3 opengl32 ${Boost_LIBRARIES} ${FREETYPE_LIBRARIES})
else()
	target_link_libraries(QuaternionViewer ${Boost_LIBRARIES} glfw GL ${FREETYPE_LIBRARIES})
endif()
