cmake_minimum_required(VERSION 3.14)
project(Compiler VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tokenizer)
add_subdirectory(parser)
add_subdirectory(generator)

add_executable(
  Compiler
  main.cpp
)

add_dependencies(
  Compiler
  Tokenizer
  Parser
  Generator
)

target_link_libraries(
  Compiler
  Tokenizer
  Parser
  Generator
)