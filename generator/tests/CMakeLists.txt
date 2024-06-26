cmake_minimum_required(VERSION 3.14)
project(GeneratorTests VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TESTS_DIR ${PROJECT_SOURCE_DIR})
configure_file(variables.h.in variables.h @ONLY)

add_executable(
  GeneratorTests
  GeneratorTests.cpp
)

add_dependencies(
  GeneratorTests
  ProgrammObjectModel
  Generator
)

target_link_libraries(
  GeneratorTests
  Generator
  GTest::gtest_main
  ${PROJECT_SOURCE_DIR}/../../parser/build/libParser.a
  ${PROJECT_SOURCE_DIR}/../../parser/build/grammar/libGrammar.a
  ${PROJECT_SOURCE_DIR}/../../tokenizer/build/libTokenizer.a
)

target_include_directories(GeneratorTests PRIVATE ${PROJECT_BINARY_DIR})

include(GoogleTest)
gtest_discover_tests(GeneratorTests)