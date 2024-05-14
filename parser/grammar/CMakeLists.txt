cmake_minimum_required(VERSION 3.14)

add_library(
  Grammar
  SHARED STATIC
  Grammar.cpp
  ./parserEngin/ParserEngin.cpp
  ./parserEngin/syntaxTree/SyntaxTree.cpp
  ../../tokenizer/Token.cpp
)