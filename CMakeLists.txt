cmake_minimum_required(VERSION 3.14)
project(graph_library_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

include(GoogleTest)
enable_testing()

add_executable(tests
    test_undirected_graph.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(tests PRIVATE
    GTest::gtest_main
    Threads::Threads
)

gtest_discover_tests(tests)

add_executable(graph
    main.cpp
)

add_executable(benchmark
    benchmark.cpp
)