cmake_minimum_required(VERSION 3.14)
project(SyntaxTreeObjectModel VERSION 0.1.0)

add_library(
  SyntaxTreeObjectModel
  SHARED STATIC
  Programm.cpp 
  FunctionDefinition.cpp
  ./expression/BaseExpression.cpp
  ./expression/BinaryOperandExpression.cpp
  ./expression/ExpressionFactory.cpp
  ./expression/FunctionCall.cpp
  ./expression/RowValue.cpp
  ./expression/UnaryOperandExpression.cpp
  ./expression/Variable.cpp
  ./operator/OperatorFactory.h
  ./operator/OperatorIf.h
  ./operator/OperatorWhile.h
  ./operator/OperatorReturn.h
  ./operator/AssignmentOperator.h
  ./operator/VariableDefinitionOperator.h
  TypeCheckErrors.cpp
  common.h
  identifiersScope/IdentifiersScope.cpp
  ../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.cpp
)