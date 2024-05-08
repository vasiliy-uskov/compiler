#pragma once
#include <list>
#include <functional>
#include <optional>
#include "ParserEngin.h"

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

    Breaket,
    Punctuation,
    Keyword,
    Identifier,
    IntValue,
    FloatValue,
    Type,
};

std::ostream& operator<<(std::ostream& os, const SyntaxRule type);

ParsingResult<SyntaxRule> parseProgramm(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionsList(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunction(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseType(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseDefinitionArgumentsList(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseDefinitionArgument(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseMainBody(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionOperatorsList(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionIfOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionWhileOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseReturnOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseOperatorsList(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseIfOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseWhileOperator(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseVariableDefinition(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseAssignment(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFunctionCall(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseCallArgumentsList(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseCallArgument(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseExpression(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseExpression1(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseExpression2(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseIdentifier(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseIntValue(ParserEngin<SyntaxRule>&, const TokenIterator&);
ParsingResult<SyntaxRule> parseFloatValue(ParserEngin<SyntaxRule>&, const TokenIterator&);

ParseFn<SyntaxRule> makeBreaketParser(ParserEngin<SyntaxRule>&, const std::string&);
ParseFn<SyntaxRule> makePunctuationParser(ParserEngin<SyntaxRule>&, const std::string&);
ParseFn<SyntaxRule> makeKeywordParser(ParserEngin<SyntaxRule>&, const std::string&);
ParseFn<SyntaxRule> makeOperatorTokenParser(ParserEngin<SyntaxRule>&, std::string&);