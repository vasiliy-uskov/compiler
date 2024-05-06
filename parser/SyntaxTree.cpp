#pragma once
#include <iostream>
#include "SyntaxTree.h"


SyntaxTree::SyntaxTree(SyntaxRule rule, const std::list<Token> & tokens)
        : rule(rule), tokens(tokens), children({}) {}
SyntaxTree::SyntaxTree(SyntaxRule rule, const std::list<Token> & tokens, const std::vector<SyntaxTree> & _children)
        : rule(rule), tokens(tokens), children(_children) {}

std::ostream& operator<<(std::ostream& os, const SyntaxRule type)
{
    switch(type)
    {
        case SyntaxRule::Program:
            return os << std::string("Program");
        case SyntaxRule::MainBody:
            return os << std::string("MainBody");
        case SyntaxRule::FunctionsList:
            return os << std::string("FunctionsList");
        case SyntaxRule::Function:
            return os << std::string("Function");
        case SyntaxRule::DefinitionArgumentsList:
            return os << std::string("DefinitionArgumentsList");
        case SyntaxRule::DefinitionArgument:
            return os << std::string("DefinitionArgument");
        case SyntaxRule::CallArgumentsList:
            return os << std::string("CallArgumentsList");
        case SyntaxRule::CallArgument:
            return os << std::string("CallArgument");
        case SyntaxRule::FunctionOperatorsList:
            return os << std::string("FunctionOperatorsList");
        case SyntaxRule::FunctionOperator:
            return os << std::string("FunctionOperator");
        case SyntaxRule::OperatorsList:
            return os << std::string("OperatorsList");
        case SyntaxRule::Operator:
            return os << std::string("Operator");
        case SyntaxRule::OperatorIf:
            return os << std::string("OperatorIf");
        case SyntaxRule::OperatorWhile:
            return os << std::string("OperatorWhile");
        case SyntaxRule::OperatorReturn:
            return os << std::string("OperatorReturn");
        case SyntaxRule::VariableDefinition:
            return os << std::string("VariableDefinition");
        case SyntaxRule::AssignmentOperator:
            return os << std::string("AssignmentOperator");
        case SyntaxRule::FunctionCall:
            return os << std::string("FunctionCall");
        case SyntaxRule::Expression:
            return os << std::string("Expression");
        case SyntaxRule::Breaket:
            return os << std::string("Breaket");
        case SyntaxRule::Punctuation:
            return os << std::string("Punctuation");
        case SyntaxRule::Keyword:
            return os << std::string("Keyword");
        case SyntaxRule::Identifier:
            return os << std::string("Identifier");
        case SyntaxRule::IntValue:
            return os << std::string("IntValue");
        case SyntaxRule::FloatValue:
            return os << std::string("FloatValue");
        case SyntaxRule::Type:
            return os << std::string("Type");
    }

    return os;
}