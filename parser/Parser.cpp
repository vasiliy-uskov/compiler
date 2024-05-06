#pragma once
#include "SyntaxTree.h"
#include "Parser.h"

ParsingResult processSyntaxRuleVariant(SyntaxRule rule, const TokenIterator & startToken, const std::list<ParseFn> & rules)
{
    std::vector<SyntaxTree> children = {};
    TokenIterator currentToken = startToken;
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        auto [ruleResult, ruleEndToken] = (*rule)(currentToken);
        if (!ruleResult.has_value())
        {
            return {std::nullopt, currentToken};
        }
        children.push_back(ruleResult.value());
        currentToken = ruleEndToken;
    }
    return {
        SyntaxTree(rule, {startToken, currentToken}, children),
        currentToken
    };
}

ParsingResult processSyntaxRule(SyntaxRule rule, const TokenIterator & startToken, const std::list<std::list<ParseFn>> & ruleVariants)
{
    for (auto ruleVariant : ruleVariants)
    {
        auto result = processSyntaxRuleVariant(rule, startToken, ruleVariant);
        if (result.first.has_value())
        {
            return result;
        }
    }
    return {std::nullopt, startToken};
}

ParsingResult parseToken(SyntaxRule rule, TokenIterator tokenIt, std::function<bool(const Token&)> matcher)
{
    const auto token = *tokenIt;
    const auto nextTokenIt = std::next(tokenIt, 1);
    if (matcher(token))
    {
        return ParsingResult(
            SyntaxTree(rule, {tokenIt, nextTokenIt}),
            nextTokenIt
        );
    }
    return ParsingResult(std::nullopt, tokenIt);
    
}

ParseFn makeTokenParserByValue(SyntaxRule rule, const std::string & tokenValue)
{
    return [rule, tokenValue](TokenIterator tokenIt) {
        return parseToken(rule, tokenIt, [tokenValue](const Token & token) {
            return token.value == tokenValue;
        });
    };
}

ParseFn makeTokenParserByType(SyntaxRule rule, TokenType tokenType)
{
    return [rule, tokenType](TokenIterator tokenIt) {
        return parseToken(rule, tokenIt, [tokenType](const Token & token) {
            return token.type == tokenType;
        });
    };
}

ParsingResult parseProgramm(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Program, startToken, {
        {parseFunctionsList, parseMainBody},
        {parseMainBody}
    });
}

ParsingResult parseFunctionsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionsList, startToken, {
        {parseFunction, parseFunctionsList},
        {parseFunction}
    });
}

ParsingResult parseFunction(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Function, startToken, {
        {
            parseType,
            parseIdentifier,
            makeBreaketParser("("),
            parseDefinitionArgumentsList,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makeBreaketParser("}")
        }, {
            makeKeywordParser("void"),
            parseIdentifier,
            makeBreaketParser("("),
            parseDefinitionArgumentsList,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makeBreaketParser("}")
        },
    });
}

ParseFn makeBreaketParser(const std::string & breaket)
{
    return makeTokenParserByValue(SyntaxRule::Breaket, breaket);
}

ParseFn makePunctuationParser(const std::string & punctuation)
{
    return makeTokenParserByValue(SyntaxRule::Punctuation, punctuation);
}

ParseFn makeKeywordParser(const std::string & keyword)
{
    return makeTokenParserByValue(SyntaxRule::Keyword, keyword);
}

ParseFn makeOperatorTokenParser(const std::string & operatorTokenValue)
{
    return makeTokenParserByValue(SyntaxRule::Keyword, operatorTokenValue);
}

ParsingResult parseType(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Type, startToken, {
        {makeKeywordParser("int")},
        {makeKeywordParser("float")},
        {makeKeywordParser("bool")}
    });
}

ParsingResult parseIdentifier(TokenIterator startToken)
{
    return makeTokenParserByType(SyntaxRule::Identifier, TokenType::Identifier)(startToken);
}

ParsingResult parseIntValue(TokenIterator startToken)
{
    return makeTokenParserByType(SyntaxRule::IntValue, TokenType::IntValue)(startToken);
}

ParsingResult parseFloatValue(TokenIterator startToken)
{
    return makeTokenParserByType(SyntaxRule::FloatValue, TokenType::FloatValue)(startToken);
}


ParsingResult parseDefinitionArgumentsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::DefinitionArgumentsList, startToken, {
        {parseDefinitionArgument, makePunctuationParser(","), parseDefinitionArgumentsList},
        {parseDefinitionArgument}
    });
}

ParsingResult parseDefinitionArgument(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::DefinitionArgumentsList, startToken, {
        parseType, parseIdentifier
    });
}

ParsingResult parseMainBody(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::MainBody, startToken, {
        {
            makeKeywordParser("main"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makeBreaketParser("}")
        },
        {
            makeKeywordParser("main"),
            makeBreaketParser("{"),
            makeBreaketParser("}")
        }
    });
}

ParsingResult parseFunctionOperatorsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionOperatorsList, startToken, {
        {parseFunctionOperator, parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult parseFunctionOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionOperator, startToken, {
        {parseVariableDefinition, makePunctuationParser(";")},
        {parseAssignment, makePunctuationParser(";")},
        {parseFunctionCall, makePunctuationParser(";")},
        {parseFunctionIfOperator},
        {parseFunctionWhileOperator},
        {parseReturnOperator, makePunctuationParser(";")},
        {makePunctuationParser(";")}
    });
}

ParsingResult parseFunctionIfOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, startToken, {
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makeBreaketParser("}"),
            makeKeywordParser("else"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makeBreaketParser("}")
        },
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makeBreaketParser("}"),
        }
    });
   
}

ParsingResult parseFunctionWhileOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, startToken, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseFunctionOperatorsList,
        makeBreaketParser("}")
    });
}

ParsingResult parseReturnOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorReturn, startToken, {
        makeKeywordParser("return"),
        parseExpression,
    });
}

ParsingResult parseOperatorsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorsList, startToken, {
        {parseOperator, parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult parseOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Operator, startToken, {
        {parseVariableDefinition, makePunctuationParser(";")},
        {parseAssignment, makePunctuationParser(";")},
        {parseFunctionCall, makePunctuationParser(";")},
        {parseIfOperator},
        {parseWhileOperator},
        {makePunctuationParser(";")}
    });
}

ParsingResult parseIfOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, startToken, {
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makeBreaketParser("}"),
            makeKeywordParser("else"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makeBreaketParser("}")
        },
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makeBreaketParser("}")
        }
    });
   
}

ParsingResult parseWhileOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, startToken, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseOperatorsList,
        makeBreaketParser("}")
    });
}

ParsingResult parseVariableDefinition(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::VariableDefinition, startToken, {
        {parseType, parseIdentifier, makeOperatorTokenParser("="), parseExpression},
        {parseType, parseIdentifier}
    });
}

ParsingResult parseAssignment(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, startToken, {
        parseIdentifier, makeOperatorTokenParser("="), parseExpression
    });
}

ParsingResult parseFunctionCall(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, startToken, {
        parseIdentifier, makeBreaketParser("("), parseCallArgumentsList, makeBreaketParser(")")
    });
}

ParsingResult parseCallArgumentsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::CallArgumentsList, startToken, {
        {parseExpression, makePunctuationParser(","), parseCallArgumentsList},
        {parseExpression}
    });
}

ParsingResult parseExpression(TokenIterator startToken)
{
    return parseExpression1(startToken);
}

ParsingResult parseExpression1(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Expression, startToken, {
        {parseExpression2, makeOperatorTokenParser("||"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("+"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("-"), parseExpression1},
        {makeOperatorTokenParser("-"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("&&"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("*"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("/"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("=="), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("<"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser("<="), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(">"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(">="), parseExpression1},
        {parseExpression2}
    });    
}

ParsingResult parseExpression2(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Expression, startToken, {
        {makeKeywordParser("true")},
        {makeKeywordParser("false")},
        {parseIntValue},
        {parseFloatValue},
        {parseFunctionCall},
        {parseIdentifier},
        {makeBreaketParser("("), parseExpression1, makeBreaketParser(")")}
    });  
}

SyntaxTree Parser::parse(std::list<Token> tokens)
{
    auto [result, endTokentIt] = parseProgramm(tokens.begin());
    if (result.has_value())
    {
        return result.value();
    }
    else
    {
        throw std::exception();
    }
}