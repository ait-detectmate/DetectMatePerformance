cmake_minimum_required(VERSION 3.25)
project(detectmateperformance)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Arrow 23 and later require C++20.
FetchContent_Declare(
  apache_arrow
  URL https://archive.apache.org/dist/arrow/arrow-22.0.0/apache-arrow-22.0.0.tar.gz
  URL_HASH SHA512=8ec9ddaf7917c0e35c8bb32831fe6ea0a7b81de5723828a1289ba1b9e104b42af688d0f427a0ceff6f617d5f7ac67769431184b137e54f6987779e467c59d3ec
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  SOURCE_SUBDIR cpp
)

block()
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()
  set(ARROW_BUILD_STATIC ON)
  set(ARROW_POSITION_INDEPENDENT_CODE ON)
  set(ARROW_DEPENDENCY_SOURCE AUTO)
  set(ARROW_SIMD_LEVEL NONE)
  set(ARROW_RUNTIME_SIMD_LEVEL NONE)
  FetchContent_MakeAvailable(apache_arrow)

  target_include_directories(
    arrow_static SYSTEM INTERFACE
    "$<BUILD_INTERFACE:${apache_arrow_SOURCE_DIR}/cpp/src>"
    "$<BUILD_INTERFACE:${apache_arrow_BINARY_DIR}/src>"
  )
endblock()

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

add_library(
  drain_op
  src/detectmateperformance/_core/parsers/drain.h
  src/detectmateperformance/_core/parsers/drain.cpp
)

add_library(
  auto_parser_op
  src/detectmateperformance/_core/parsers/auto_parser.h
  src/detectmateperformance/_core/parsers/auto_parser.cpp
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
  drain_op
  auto_parser_op
)

add_executable(
  test_c
  tests/test_c/test_message.cpp
)

add_executable(
  test_type
  tests/test_c/test_type.cpp
)
target_compile_definitions(
  test_type PRIVATE
  TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data"
)

add_executable(
  test_matcher_tree
  tests/test_c/test_matcher_tree.cpp
)

add_executable(
  test_matcher_vars
  tests/test_c/test_matcher_vars.cpp
)

add_executable(
  test_drain_op
  tests/test_c/test_drain_op.cpp
)

add_executable(
  test_auto_parser
  tests/test_c/test_auto_parser.cpp
)
target_compile_definitions(
  test_auto_parser PRIVATE
  TEST_TEMPLATES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/detectmateperformance/_templates"
)

add_executable(
  test_aecid_pg
  tests/test_c/test_aecid_pg.cpp
  src/detectmateperformance/_core/parsers/aecid_pg.cpp
)
target_link_libraries(test_aecid_pg PRIVATE GTest::gtest_main)

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
  src/detectmateperformance/_core/parsers/drain.h
  src/detectmateperformance/_core/parsers/drain.cpp
  src/detectmateperformance/_core/parsers/auto_parser.h
  src/detectmateperformance/_core/parsers/auto_parser.cpp
  src/detectmateperformance/_core/parsers/aecid_pg.h
  src/detectmateperformance/_core/parsers/aecid_pg.cpp
  src/detectmateperformance/_core/parsers/brain.h
  src/detectmateperformance/_core/parsers/brain.cpp
)
target_link_libraries(bind_class PRIVATE arrow_static)

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

target_link_libraries(
  test_drain_op
  GTest::gtest_main
  drain_op
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
  test_auto_parser
  GTest::gtest_main
  auto_parser_op
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
gtest_discover_tests(test_drain_op)
gtest_discover_tests(test_auto_parser)
gtest_discover_tests(test_aecid_pg)
