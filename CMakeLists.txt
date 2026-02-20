cmake_minimum_required(VERSION 3.14)
project(DetectMatePerformance)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_test(
  NAME memory_leak_test 
  COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_c
)


add_library(
  message
  DetectMatePerformance/src/_core/message.h
  DetectMatePerformance/src/_core/messages.cpp
)

add_executable(
  test_c
  DetectMatePerformance/tests/test_c/test_message.cpp
)

target_link_libraries(
  test_c
  GTest::gtest_main
  message
)

include(GoogleTest)
gtest_discover_tests(test_c)
