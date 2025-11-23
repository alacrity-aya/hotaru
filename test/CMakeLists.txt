# --- test/CMakeLists.txt ---
file(GLOB TEST_SOURCES
    CONFIGURE_DEPENDS
    "*_test.cpp"
)

# SET(TEST_SOURCES btree_test.cpp file_io_test.cpp)

add_executable(hotaru_tests ${TEST_SOURCES})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(hotaru_tests PRIVATE 
    hotaru_core 
    GTest::gtest GTest::gtest_main
)

target_include_directories(hotaru_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/src 
)

add_test(NAME AllHotaruTests COMMAND hotaru_tests)
