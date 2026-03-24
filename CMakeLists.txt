cmake_minimum_required(VERSION 3.20)
project(InMemoryDB)

set(CMAKE_CXX_STANDARD 20)

add_library(in_memory_db src/in_memory_db.cpp)
target_include_directories(in_memory_db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(db_tests tests/test_main.cpp)
target_link_libraries(db_tests PRIVATE in_memory_db GTest::gtest_main)

enable_testing()
add_test(NAME AllTests COMMAND db_tests)


add_executable(db_cli src/main.cpp)
target_link_libraries(db_cli PRIVATE in_memory_db)
