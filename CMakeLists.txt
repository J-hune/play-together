cmake_minimum_required(VERSION 3.26)

project(play-together)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Find SDL2 library
find_package(SDL2 REQUIRED)

# Find SDL_ttf library
find_package(SDL2_ttf REQUIRED)

# Find SDL2_image library
find_package(SDL2_image REQUIRED)

# Find SDL2_mixer library
find_package(SDL2_mixer REQUIRED)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

if(NOT SDL2_TTF_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

if(NOT SDL2_IMAGE_FOUND)
    message(FATAL_ERROR "SDL2_image not found!")
endif()

if(NOT SDL2_MIXER_FOUND)
    message(FATAL_ERROR "SDL2_mixer not found!")
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

# Set assets directory
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Copy assets to binary directory
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Create 'saves' directory in the binary directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/saves)

# Automatic retrieval of source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Automatic retrieval of header files
file(GLOB_RECURSE HEADERS include/*.h)

# Automatic retrieval of dependencies
file(GLOB_RECURSE DEPENDENCIES ${DEPENDENCIES_DIR}/*)

# Create the executable
add_executable(play-together WIN32 ${SOURCES} ${HEADERS} ${DEPENDENCIES})

# Link libraries
target_link_libraries(play-together ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})

# Check if it's Windows and add Ws2_32.lib
if (WIN32)
    target_link_libraries(play-together Ws2_32.lib)
endif()

# Set the output directory for the executable
set_target_properties(play-together PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
