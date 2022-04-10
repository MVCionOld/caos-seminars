cmake_minimum_required(VERSION 3.15)

project(00-fuse)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED fuse3)

include_directories(${FUSE_INCLUDE_DIRS})
add_executable(00-fuse main.c)
target_link_libraries(00-fuse ${FUSE_LIBRARIES})