cmake_minimum_required (VERSION 2.6)

project(AttVis)

# Options
option(EMSCRIPTEN off)

# Check if Emscripten
if(${CMAKE_SYSTEM} STREQUAL Emscripten-1)
	option(EMSCRIPTEN on)
endif()

# Include headers
include_directories(include)
include_directories(PulsarEngine/Pulsar/include)
include_directories(PulsarEngine/lib)

# Compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp -Wall -O3 -ffast-math")
if(EMSCRIPTEN)
	# see https://github.com/kripken/emscripten/blob/master/src/settings.js
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
		--closure 1 --llvm-lto 0 \
		-s NO_EXIT_RUNTIME=1 -s USE_SDL=2 -s USE_LIBPNG=1 -s USE_ZLIB=1")
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
if(EMSCRIPTEN)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Dependencies
add_subdirectory(PulsarEngine)

# Sources
file(GLOB SOURCES "src/*.cpp")

# Target
add_executable(attvis ${SOURCES})

# Linking
link_directories(${CMAKE_BINARY_DIR}/lib)
if(UNIX)
	target_link_libraries(attvis Pulsar)
elseif(WIN32)
	target_link_libraries(attvis Pulsar)
endif()

# Copy data
add_custom_target(link_target ALL COMMAND ${CMAKE_COMMAND} -E
	create_symlink ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/data@/data")
endif()
# file(GLOB data_files "${CMAKE_BINARY_DIR}/data/*/*")
# add_custom_target(data_target DEPENDS data_files)
# add_dependencies(attvis data_target)
