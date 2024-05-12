cmake_minimum_required(VERSION 3.14)
project(GRAMMAR VERSION 0.1.0)

add_library(
  Grammar
  SHARED STATIC
  Grammar.cpp
  ./parserEngin/ParserEngin.cpp
  ./parserEngin/syntaxTree/SyntaxTree.cpp
  ../../tokenizer/Token.cpp
)