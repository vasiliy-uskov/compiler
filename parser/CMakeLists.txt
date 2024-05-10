cmake_minimum_required(VERSION 3.14)
project(Parser VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(grammar)

add_library(
  parser
  Parser.cpp
  ./utils/JSLikeVectorAdapter.h
  ../tokenizer/Token.cpp
)

target_link_libraries(
  parser
  grammar
)