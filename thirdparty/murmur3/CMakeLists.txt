cmake_minimum_required(VERSION 2.8.8)
project(murmur_builder CXX)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    murmur3
    PREFIX ${CMAKE_BINARY_DIR}/murmur3
    GIT_REPOSITORY https://github.com/PeterScott/murmur3.git
    GIT_TAG dae94be0c0f54a399d23ea6cbe54bca5a4e93ce4
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make shared
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE 1
    LOG_DOWNLOAD ON
   )

set(MURMUR3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/murmur3/src/murmur3/ CACHE INTERNAL
  "murmur3 include directory")
set(MURMUR3_LIBRARIES ${CMAKE_BINARY_DIR}/murmur3/src/murmur3/libmurmur3.so CACHE INTERNAL
  "murmur3 library path")
