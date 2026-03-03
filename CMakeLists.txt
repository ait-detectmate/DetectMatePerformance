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

# Current not working, figure it out why! TODO: Fix pybind11
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v3.0.1  # Use the latest stable version
)

FetchContent_MakeAvailable(pybind11)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_test(
  NAME memory_leak_test 
  COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_c
)

add_library(
  aux
  DetectMatePerformance/src/_core/aux.h
  DetectMatePerformance/src/_core/aux.cpp
)

add_library(
  templates
  DetectMatePerformance/src/_core/_type/templates.h
  DetectMatePerformance/src/_core/_type/templates.cpp
)

add_library(
  parsed
  DetectMatePerformance/src/_core/_type/parsed.h
  DetectMatePerformance/src/_core/_type/parsed.cpp
)

add_library(
  tree
  DetectMatePerformance/src/_core/template_matcher/tree.h
  DetectMatePerformance/src/_core/template_matcher/tree.cpp
)

add_library(
  variable
  DetectMatePerformance/src/_core/template_matcher/variables.h
  DetectMatePerformance/src/_core/template_matcher/variables.cpp
)

add_library(
  tree_op
  DetectMatePerformance/src/_core/template_matcher/tree_op.h
  DetectMatePerformance/src/_core/template_matcher/tree_op.cpp
)

add_library(
  match_tree
  DetectMatePerformance/src/_core/template_matcher/match_tree.h
  DetectMatePerformance/src/_core/template_matcher/match_tree.cpp
)

add_executable(
  test_c
  DetectMatePerformance/tests/test_c/test_message.cpp
)

pybind11_add_module(
  message_class 
  DetectMatePerformance/src/_core/message_bind.cpp
  DetectMatePerformance/src/_core/aux.h
  DetectMatePerformance/src/_core/aux.cpp
  DetectMatePerformance/src/_core/_type/templates.h
  DetectMatePerformance/src/_core/_type/templates.cpp
  DetectMatePerformance/src/_core/_type/parsed.h
  DetectMatePerformance/src/_core/_type/parsed.cpp
  DetectMatePerformance/src/_core/template_matcher/tree.h
  DetectMatePerformance/src/_core/template_matcher/tree.cpp
  DetectMatePerformance/src/_core/template_matcher/variables.h
  DetectMatePerformance/src/_core/template_matcher/variables.cpp
  DetectMatePerformance/src/_core/template_matcher/tree_op.h
  DetectMatePerformance/src/_core/template_matcher/tree_op.cpp
  DetectMatePerformance/src/_core/template_matcher/match_tree.h
  DetectMatePerformance/src/_core/template_matcher/match_tree.cpp
)

target_link_libraries(
  test_c
  GTest::gtest_main
  templates
  parsed
  tree 
  variable
  tree_op
  match_tree
  aux
)


include(GoogleTest)
gtest_discover_tests(test_c)
