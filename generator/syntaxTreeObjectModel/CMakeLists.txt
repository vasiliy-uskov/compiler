cmake_minimum_required(VERSION 3.14)
project(SyntaxTreeObjectModel VERSION 0.1.0)

add_library(
  SyntaxTreeObjectModel
  SHARED STATIC
  Programm.cpp 
  Operator.cpp 
  FunctionDefinition.cpp
  Expression.cpp
  TypeCheckErrors.cpp
  common.h
  identifiersScope/IdentifiersScope.cpp
  ../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.cpp
)