cmake_minimum_required(VERSION 3.26)
project(hotaru)

# static lib
set(HOTARU_CORE_SRCS
    src/index/btree.cpp
)

add_library(hotaru_core STATIC 
    ${HOTARU_CORE_SRCS}
)

target_include_directories(hotaru_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/btree.h")
target_include_directories(hotaru_core INTERFACE ${PARALLEL_HASHMAP_INCLUDE_DIRS})

# cpptrace
find_package(cpptrace CONFIG REQUIRED)
target_link_libraries(hotaru_core PRIVATE cpptrace::cpptrace)

add_executable(hotaru src/main.cpp)
target_link_libraries(hotaru PRIVATE 
    hotaru_core 
)


# test
enable_testing()
add_subdirectory(test)
