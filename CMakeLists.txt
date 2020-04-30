cmake_minimum_required(VERSION 3.1)
project(rec VERSION 1.0)

option(BUILD_APPS "" OFF)
add_subdirectory(Bento4)
add_subdirectory(extractor)
