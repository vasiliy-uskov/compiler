cmake_minimum_required(VERSION 3.14)
project(Tokenizer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tests)

add_library(Tokenizer STATIC
  Token.cpp
  Tokenizer.cpp
)