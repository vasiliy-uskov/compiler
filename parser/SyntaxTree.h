#pragma once
#include <vector>
#include <list>
#include "../tokenizer/Token.h"

enum SyntaxRule {
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
    LogicalExpression,
    ArithmeticExpression,

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
    const std::list<Token> tokens;
    const SyntaxRule rule;

    SyntaxTree(SyntaxRule, const std::list<Token> &);
    SyntaxTree(SyntaxRule, const std::list<Token> &, const std::vector<SyntaxTree> &);
};