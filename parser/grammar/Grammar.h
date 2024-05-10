#pragma once
#include "./parserEngin/ParserEngin.h"

ParsingResult parseProgramm(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionsList(ParserEngin&, const TokenIterator&);
ParsingResult parseFunction(ParserEngin&, const TokenIterator&);
ParsingResult parseType(ParserEngin&, const TokenIterator&);
ParsingResult parseDefinitionArgumentsList(ParserEngin&, const TokenIterator&);
ParsingResult parseDefinitionArgument(ParserEngin&, const TokenIterator&);
ParsingResult parseMainBody(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionOperatorsList(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionIfOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionWhileOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseReturnOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseOperatorsList(ParserEngin&, const TokenIterator&);
ParsingResult parseOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseIfOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseWhileOperator(ParserEngin&, const TokenIterator&);
ParsingResult parseVariableDefinition(ParserEngin&, const TokenIterator&);
ParsingResult parseAssignment(ParserEngin&, const TokenIterator&);
ParsingResult parseFunctionCall(ParserEngin&, const TokenIterator&);
ParsingResult parseCallArgumentsList(ParserEngin&, const TokenIterator&);
ParsingResult parseCallArgument(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression1(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression2(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression3(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression4(ParserEngin&, const TokenIterator&);
ParsingResult parseExpression5(ParserEngin&, const TokenIterator&);
ParsingResult parseIdentifier(ParserEngin&, const TokenIterator&);
ParsingResult parseIntValue(ParserEngin&, const TokenIterator&);
ParsingResult parseFloatValue(ParserEngin&, const TokenIterator&);

ParseFn makeBreaketParser(ParserEngin&, const std::string&);
ParseFn makePunctuationParser(ParserEngin&, const std::string&);
ParseFn makeKeywordParser(ParserEngin&, const std::string&);
ParseFn makeOperatorTokenParser(ParserEngin&, std::string&);