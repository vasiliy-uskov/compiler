#pragma once
#include <list>
#include <functional>
#include <optional>
#include "../tokenizer/Token.h"
#include "SyntaxTree.h"

typedef std::pair<std::optional<SyntaxTree>, std::list<Token>::iterator> ParsingResult;
typedef std::list<Token>::iterator TokenIterator;
typedef std::function<ParsingResult(std::list<Token>::iterator)> ParseFn;

ParsingResult parseProgramm(TokenIterator);
ParsingResult parseFunctionsList(TokenIterator);
ParsingResult parseFunction(TokenIterator);
ParsingResult parseType(TokenIterator);
ParsingResult parseDefinitionArgumentsList(TokenIterator);
ParsingResult parseDefinitionArgument(TokenIterator);
ParsingResult parseMainBody(TokenIterator);
ParsingResult parseFunctionOperatorsList(TokenIterator);
ParsingResult parseFunctionOperator(TokenIterator);
ParsingResult parseFunctionIfOperator(TokenIterator);
ParsingResult parseFunctionWhileOperator(TokenIterator);
ParsingResult parseReturnOperator(TokenIterator);
ParsingResult parseOperatorsList(TokenIterator);
ParsingResult parseOperator(TokenIterator);
ParsingResult parseIfOperator(TokenIterator);
ParsingResult parseWhileOperator(TokenIterator);
ParsingResult parseVariableDefinition(TokenIterator);
ParsingResult parseAssignment(TokenIterator);
ParsingResult parseFunctionCall(TokenIterator);
ParsingResult parseCallArgumentsList(TokenIterator);
ParsingResult parseCallArgument(TokenIterator);
ParsingResult parseExpression(TokenIterator);
ParsingResult parseLogicalExpression(TokenIterator);
ParsingResult parseLogicalExpression1(TokenIterator);
ParsingResult parseLogicalExpression2(TokenIterator);
ParsingResult parseLogicalExpression3(TokenIterator);
ParsingResult parseArithmeticExpression(TokenIterator);
ParsingResult parseArithmeticExpression1(TokenIterator);
ParsingResult parseArithmeticExpression2(TokenIterator);
ParsingResult parseArithmeticExpression3(TokenIterator);
ParsingResult parseIdentifier(TokenIterator);
ParsingResult parseIntValue(TokenIterator);
ParsingResult parseFloatValue(TokenIterator);

ParseFn makeBreaketParser(const std::string&);
ParseFn makePunctuationParser(const std::string&);
ParseFn makeKeywordParser(const std::string&);
ParseFn makeOperatorTokenParser(const std::string&);

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};