cmake_minimum_required(VERSION 2.8.8)
project(re2_builder CXX)
include(ExternalProject)
find_package(Git REQUIRED)
find_package(Threads REQUIRED)

set(RE2_PREFIX ${CMAKE_BINARY_DIR}/re2)
set(RE2_NAME "RE2")

ExternalProject_Add(
    re2
    PREFIX ${RE2_PREFIX}
    GIT_REPOSITORY https://github.com/google/re2.git
    GIT_TAG 2015-10-01
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE 1
    LOG_DOWNLOAD ON
   )

set(RE2_INCLUDE_DIR ${RE2_PREFIX}/src/re2/ CACHE INTERNAL
  "Re2 include directory")
set(RE2_LIBRARIES ${RE2_PREFIX}/src/re2/obj/so/libre2.so CACHE INTERNAL
  "Re2 library path")
set(RE2_STATIC_LIBRARIES ${RE2_PREFIX}/src/re2/obj/libre2.a
  ${CMAKE_THREAD_LIBS_INIT} CACHE INTERNAL
  "Re2 static library path.")
