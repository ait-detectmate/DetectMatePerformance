cmake_minimum_required(VERSION 3.14)
project(detectmateperformance)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

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
  src/detectmateperformance/_core/aux.h
  src/detectmateperformance/_core/aux.cpp
)

add_library(
  templates
  src/detectmateperformance/_core/_type/templates.h
  src/detectmateperformance/_core/_type/templates.cpp
)

add_library(
  parsedelement
  src/detectmateperformance/_core/_type/element.h
  src/detectmateperformance/_core/_type/element.cpp
)

add_library(
  parsed
  src/detectmateperformance/_core/_type/parsed.h
  src/detectmateperformance/_core/_type/parsed.cpp
)

add_library(
  tree
  src/detectmateperformance/_core/template_matcher/tree.h
  src/detectmateperformance/_core/template_matcher/tree.cpp
)

add_library(
  variable
  src/detectmateperformance/_core/template_matcher/variables.h
  src/detectmateperformance/_core/template_matcher/variables.cpp
)

add_library(
  tree_op
  src/detectmateperformance/_core/template_matcher/tree_op.h
  src/detectmateperformance/_core/template_matcher/tree_op.cpp
)

add_library(
  match_tree
  src/detectmateperformance/_core/template_matcher/match_tree.h
  src/detectmateperformance/_core/template_matcher/match_tree.cpp
)

# match_tree depends on variable and tree_op; make those transitive so
# consumers of match_tree (executables/tests) don't need to worry about
# static library link ordering.
target_link_libraries(
  match_tree
  PUBLIC
  variable
  tree_op
  tree
)

add_executable(
  test_c
  tests/test_c/test_message.cpp
)

add_executable(
  test_type
  tests/test_c/test_type.cpp
)

add_executable(
  test_matcher_tree
  tests/test_c/test_matcher_tree.cpp
)

add_executable(
  test_matcher_vars
  tests/test_c/test_matcher_vars.cpp
)

pybind11_add_module(
  bind_class
  src/detectmateperformance/_core/bind_class.cpp
  src/detectmateperformance/_core/aux.h
  src/detectmateperformance/_core/aux.cpp
  src/detectmateperformance/_core/_type/templates.h
  src/detectmateperformance/_core/_type/templates.cpp
  src/detectmateperformance/_core/_type/element.h
  src/detectmateperformance/_core/_type/element.cpp
  src/detectmateperformance/_core/_type/parsed.h
  src/detectmateperformance/_core/_type/parsed.cpp
  src/detectmateperformance/_core/template_matcher/tree.h
  src/detectmateperformance/_core/template_matcher/tree.cpp
  src/detectmateperformance/_core/template_matcher/variables.h
  src/detectmateperformance/_core/template_matcher/variables.cpp
  src/detectmateperformance/_core/template_matcher/tree_op.h
  src/detectmateperformance/_core/template_matcher/tree_op.cpp
  src/detectmateperformance/_core/template_matcher/match_tree.h
  src/detectmateperformance/_core/template_matcher/match_tree.cpp
)

target_link_libraries(
  test_c
  GTest::gtest_main
  templates
  parsedelement
  parsed
  tree
  variable
  tree_op
  match_tree
  aux
)

target_link_libraries(
  test_type
  GTest::gtest_main
  templates
  parsedelement
  parsed
  aux
)

target_link_libraries(
  test_matcher_tree
  GTest::gtest_main
  tree
)

target_link_libraries(
  test_matcher_vars
  GTest::gtest_main
  templates
  parsedelement
  parsed
  tree
  variable
  tree_op
  match_tree
  aux
)

include(GoogleTest)
gtest_discover_tests(test_c)
gtest_discover_tests(test_type)
gtest_discover_tests(test_matcher_tree)
gtest_discover_tests(test_matcher_vars)
