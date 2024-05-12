cmake_minimum_required(VERSION 3.14)
project(ParserTests VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  ParserTests
  ParserTests.cpp
  ../../tokenizer/Tokenizer.cpp
)

add_dependencies(
  ParserTests
  Parser
  Grammar
)

target_link_libraries(
  ParserTests
  Parser
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ParserTests)