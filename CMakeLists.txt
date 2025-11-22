cmake_minimum_required(VERSION 3.26)
project(hotaru)

add_executable(hotaru main.cpp)

find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/btree.h")
target_include_directories(hotaru PRIVATE ${PARALLEL_HASHMAP_INCLUDE_DIRS})

# test
enable_testing()

add_subdirectory(test)
