#pragma once
#include <list>
#include <functional>
#include <optional>
#include "SyntaxTree.h"
#include "TokenProvider.h"

typedef std::pair<std::optional<SyntaxTree>, TokenProvider> ParsingResult;
typedef std::function<ParsingResult(TokenProvider)> ParseFn;

ParsingResult parseProgramm(TokenProvider);
ParsingResult parseFunctionsList(TokenProvider);
ParsingResult parseFunction(TokenProvider);
ParsingResult parseType(TokenProvider);
ParsingResult parseDefinitionArgumentsList(TokenProvider);
ParsingResult parseDefinitionArgument(TokenProvider);
ParsingResult parseMainBody(TokenProvider);
ParsingResult parseFunctionOperatorsList(TokenProvider);
ParsingResult parseFunctionOperator(TokenProvider);
ParsingResult parseFunctionIfOperator(TokenProvider);
ParsingResult parseFunctionWhileOperator(TokenProvider);
ParsingResult parseReturnOperator(TokenProvider);
ParsingResult parseOperatorsList(TokenProvider);
ParsingResult parseOperator(TokenProvider);
ParsingResult parseIfOperator(TokenProvider);
ParsingResult parseWhileOperator(TokenProvider);
ParsingResult parseVariableDefinition(TokenProvider);
ParsingResult parseAssignment(TokenProvider);
ParsingResult parseFunctionCall(TokenProvider);
ParsingResult parseCallArgumentsList(TokenProvider);
ParsingResult parseCallArgument(TokenProvider);
ParsingResult parseExpression(TokenProvider);
ParsingResult parseExpression1(TokenProvider);
ParsingResult parseExpression2(TokenProvider);
ParsingResult parseIdentifier(TokenProvider);
ParsingResult parseIntValue(TokenProvider);
ParsingResult parseFloatValue(TokenProvider);

ParseFn makeBreaketParser(const std::string&);
ParseFn makePunctuationParser(const std::string&);
ParseFn makeKeywordParser(const std::string&);
ParseFn makeOperatorTokenParser(const std::string&);

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};