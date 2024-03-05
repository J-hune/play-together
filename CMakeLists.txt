cmake_minimum_required(VERSION 3.26)

project(play-together)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

# Find SDL2 library
find_package(SDL2 REQUIRED)

# Find SDL_ttf library
find_package(SDL2_ttf REQUIRED)

# Find SDL2_gfx library
find_package(SDL2_gfx REQUIRED)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

if(NOT SDL2_TTF_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

if(NOT SDL2_GFX_FOUND)
    message(FATAL_ERROR "SDL2_gfx not found!")
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_GFX_INCLUDE_DIRS})

#Find boost library
#find_package( Boost 1.82 COMPONENTS lboost_serialization  REQUIRED )
find_package(Boost COMPONENTS serialization REQUIRED)
#FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()
#target_include_directories( play-together PRIVATE ${Boost_INCLUDE_DIR})

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
add_executable(play-together WIN32 ${SOURCES} ${HEADERS})


# Link libraries
target_link_libraries(play-together ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_GFX_LIBRARY})
target_link_libraries(play-together ${Boost_LIBRARIES})
target_link_libraries(play-together Boost::serialization)

