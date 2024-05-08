#pragma once
#include <list>
#include <functional>
#include <optional>
#include "SyntaxTree.h"
#include "TokenProvider.h"

typedef std::pair<std::optional<SyntaxTree>, TokenProvider> ParsingResult;
typedef std::function<ParsingResult(TokenProvider)> ParseFn;

ParsingResult parseProgramm(const TokenProvider&);
ParsingResult parseFunctionsList(const TokenProvider&);
ParsingResult parseFunction(const TokenProvider&);
ParsingResult parseType(const TokenProvider&);
ParsingResult parseDefinitionArgumentsList(const TokenProvider&);
ParsingResult parseDefinitionArgument(const TokenProvider&);
ParsingResult parseMainBody(const TokenProvider&);
ParsingResult parseFunctionOperatorsList(const TokenProvider&);
ParsingResult parseFunctionOperator(const TokenProvider&);
ParsingResult parseFunctionIfOperator(const TokenProvider&);
ParsingResult parseFunctionWhileOperator(const TokenProvider&);
ParsingResult parseReturnOperator(const TokenProvider&);
ParsingResult parseOperatorsList(const TokenProvider&);
ParsingResult parseOperator(const TokenProvider&);
ParsingResult parseIfOperator(const TokenProvider&);
ParsingResult parseWhileOperator(const TokenProvider&);
ParsingResult parseVariableDefinition(const TokenProvider&);
ParsingResult parseAssignment(const TokenProvider&);
ParsingResult parseFunctionCall(const TokenProvider&);
ParsingResult parseCallArgumentsList(const TokenProvider&);
ParsingResult parseCallArgument(const TokenProvider&);
ParsingResult parseExpression(const TokenProvider&);
ParsingResult parseExpression1(const TokenProvider&);
ParsingResult parseExpression2(const TokenProvider&);
ParsingResult parseIdentifier(const TokenProvider&);
ParsingResult parseIntValue(const TokenProvider&);
ParsingResult parseFloatValue(const TokenProvider&);

ParseFn makeBreaketParser(const std::string&);
ParseFn makePunctuationParser(const std::string&);
ParseFn makeKeywordParser(const std::string&);
ParseFn makeOperatorTokenParser(const std::string&);

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};