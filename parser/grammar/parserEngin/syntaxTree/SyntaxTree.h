#pragma once
#include <vector>
#include <list>
#include <iostream>
#include "../../../../tokenizer/Token.h"

enum class SyntaxRule {
    Program,
    MainBody,

    FunctionsList,
    Function,

    DefinitionArgumentsList,
    DefinitionArgument,
    CallArgumentsList,
    CallArgument,

    FunctionOperatorsList,
    FunctionOperator,
    OperatorsList,
    Operator,

    OperatorIf,
    OperatorWhile,
    OperatorReturn,
    VariableDefinition,
    AssignmentOperator,
    FunctionCall,

    Expression,

    Breaket,
    Punctuation,
    Keyword,
    Identifier,
    IntValue,
    FloatValue,
    Type,
};

class SyntaxTree
{
public:
    const std::vector<SyntaxTree> children;
    const std::vector<Token> tokens;
    const SyntaxRule rule;

    SyntaxTree(SyntaxRule, const std::vector<Token> &);
    SyntaxTree(SyntaxRule, const std::vector<Token> &, const std::vector<SyntaxTree> &);

    SyntaxTree & operator=(const SyntaxTree & tree);
};

std::ostream& operator<<(std::ostream& os, const SyntaxRule type);
bool isListRule(SyntaxRule rule);
bool isOperatorRule(SyntaxRule rule);
