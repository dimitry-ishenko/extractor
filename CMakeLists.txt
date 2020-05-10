cmake_minimum_required(VERSION 3.1)
project(extractor VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_APPS "" OFF)
add_subdirectory(Bento4)

add_definitions(-DPROJECT_VERSION=${CMAKE_PROJECT_VERSION})

set(SOURCES
    src/args.cpp
    src/args.hpp
    src/main.cpp
    src/mp4_file.cpp
    src/mp4_file.hpp
    src/mp4_track.cpp
    src/mp4_track.hpp
    src/protobuf.cpp
    src/protobuf.hpp
    src/tran_script.cpp
    src/tran_script.hpp
    src/types.hpp
)
if(WIN32)
    list(APPEND SOURCES res/extractor.rc)
endif()

set(DOCS LICENSE.md README.md)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ap4)
