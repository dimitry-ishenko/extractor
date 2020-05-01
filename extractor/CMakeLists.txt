cmake_minimum_required(VERSION 3.1)
project(extractor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/mp4_file.cpp
    src/mp4_file.hpp
    src/mp4_track.cpp
    src/mp4_track.hpp
    src/protobuf.cpp
    src/protobuf.hpp
    src/types.hpp
)
set(DOCS LICENSE.md README.md)

########################
# executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ap4)
