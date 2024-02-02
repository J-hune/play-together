cmake_minimum_required(VERSION 3.26)

project(SDL2Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

# Find SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Automatic retrieval of source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Automatic retrieval of header files
file(GLOB_RECURSE HEADERS include/*.h)

# Copy asset files with their folder structure
file(GLOB_RECURSE ASSETS_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/assets assets/*)
foreach(ASSET ${ASSETS_LIST})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/${ASSET} ${CMAKE_CURRENT_BINARY_DIR}/assets/${ASSET} COPYONLY)
endforeach()

# Copy assets to binary directory
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

# Create the executable
add_executable(SDL2Test WIN32 ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
