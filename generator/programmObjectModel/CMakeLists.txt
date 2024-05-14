cmake_minimum_required(VERSION 3.14)
project(ProgrammObjectModel VERSION 0.1.0)

add_compile_options(-Wno-return-type)

add_library(
  ProgrammObjectModel
  SHARED STATIC
  Programm.cpp 
  FunctionDefinition.cpp
  buildList.h
  TypeCheckErrors.cpp

  operator/OperatorFactory.cpp
  operator/OperatorIf.cpp
  operator/OperatorWhile.cpp
  operator/OperatorReturn.cpp
  operator/AssignmentOperator.cpp
  operator/VariableDefinitionOperator.cpp

  expression/BaseExpression.cpp
  expression/BinaryOperandExpression.cpp
  expression/ExpressionFactory.cpp
  expression/FunctionCall.cpp
  expression/RowValue.cpp
  expression/UnaryOperandExpression.cpp
  expression/Variable.cpp

  identifiersScope/IdentifiersScope.cpp
  INode.cpp
)

target_include_directories(ProgrammObjectModel PRIVATE .)