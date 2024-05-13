cmake_minimum_required(VERSION 3.14)
project(SyntaxTreeObjectModel VERSION 0.1.0)

add_library(
  SyntaxTreeObjectModel
  SHARED STATIC
  Programm.cpp 
  FunctionDefinition.cpp
  common.h
  TypeCheckErrors.cpp

  expression/BaseExpression.cpp
  expression/BinaryOperandExpression.cpp
  expression/ExpressionFactory.cpp
  expression/FunctionCall.cpp
  expression/RowValue.cpp
  expression/UnaryOperandExpression.cpp
  expression/Variable.cpp

  operator/OperatorFactory.cpp
  operator/OperatorIf.cpp
  operator/OperatorWhile.cpp
  operator/OperatorReturn.cpp
  operator/AssignmentOperator.cpp
  operator/VariableDefinitionOperator.cpp

  identifiersScope/IdentifiersScope.cpp

  ../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.cpp
)