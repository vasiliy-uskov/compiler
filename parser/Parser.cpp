#pragma once
#include "SyntaxTree.h"
#include "Parser.h"

ParsingResult processSyntaxRuleVariant(SyntaxRule rule, const TokenProvider & startProvider, const std::list<ParseFn> & rules)
{
    std::vector<SyntaxTree> children = {};
    TokenProvider currentProvider = startProvider;
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        auto [ruleResult, ruleEndProvider] = (*rule)(currentProvider);
        if (!ruleResult.has_value())
        {
            return {std::nullopt, currentProvider};
        }
        children.push_back(ruleResult.value());
        currentProvider = ruleEndProvider;
    }
    return {
        SyntaxTree(rule, {startProvider.getIt(), currentProvider.getIt()}, children),
        currentProvider
    };
}

ParsingResult processSyntaxRule(SyntaxRule rule, const TokenProvider & startToken, const std::list<std::list<ParseFn>> & ruleVariants)
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

ParsingResult parseToken(SyntaxRule rule, TokenProvider provider, std::function<bool(const Token&)> matcher)
{
    if (!provider.isEnd() && matcher(provider.get()))
    {
        const auto nextProvider = provider.next();
        return ParsingResult(
            SyntaxTree(rule, {provider.getIt(), nextProvider.getIt()}),
            nextProvider
        );
    }
    return ParsingResult(std::nullopt, provider);
    
}

ParseFn makeTokenParserByValue(SyntaxRule rule, const std::string & tokenValue)
{
    return [rule, tokenValue](TokenProvider tokenIt) {
        return parseToken(rule, tokenIt, [tokenValue](const Token & token) {
            return token.value == tokenValue;
        });
    };
}

ParseFn makeTokenParserByType(SyntaxRule rule, TokenType tokenType)
{
    return [rule, tokenType](TokenProvider tokenIt) {
        return parseToken(rule, tokenIt, [tokenType](const Token & token) {
            return token.type == tokenType;
        });
    };
}

ParsingResult parseProgramm(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::Program, startToken, {
        {parseFunctionsList, parseMainBody},
        {parseMainBody}
    });
}

ParsingResult parseFunctionsList(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionsList, startToken, {
        {parseFunction, parseFunctionsList},
        {parseFunction}
    });
}

ParsingResult parseFunction(TokenProvider startToken)
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

ParsingResult parseType(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::Type, startToken, {
        {makeKeywordParser("int")},
        {makeKeywordParser("float")},
        {makeKeywordParser("bool")}
    });
}

ParsingResult parseIdentifier(TokenProvider startToken)
{
    return makeTokenParserByType(SyntaxRule::Identifier, TokenType::Identifier)(startToken);
}

ParsingResult parseIntValue(TokenProvider startToken)
{
    return makeTokenParserByType(SyntaxRule::IntValue, TokenType::IntValue)(startToken);
}

ParsingResult parseFloatValue(TokenProvider startToken)
{
    return makeTokenParserByType(SyntaxRule::FloatValue, TokenType::FloatValue)(startToken);
}


ParsingResult parseDefinitionArgumentsList(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::DefinitionArgumentsList, startToken, {
        {parseDefinitionArgument, makePunctuationParser(","), parseDefinitionArgumentsList},
        {parseDefinitionArgument}
    });
}

ParsingResult parseDefinitionArgument(TokenProvider startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::DefinitionArgumentsList, startToken, {
        parseType, parseIdentifier
    });
}

ParsingResult parseMainBody(TokenProvider startToken)
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

ParsingResult parseFunctionOperatorsList(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionOperatorsList, startToken, {
        {parseFunctionOperator, parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult parseFunctionOperator(TokenProvider startToken)
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

ParsingResult parseFunctionIfOperator(TokenProvider startToken)
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

ParsingResult parseFunctionWhileOperator(TokenProvider startToken)
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

ParsingResult parseReturnOperator(TokenProvider startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorReturn, startToken, {
        makeKeywordParser("return"),
        parseExpression,
    });
}

ParsingResult parseOperatorsList(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorsList, startToken, {
        {parseOperator, parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult parseOperator(TokenProvider startToken)
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

ParsingResult parseIfOperator(TokenProvider startToken)
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

ParsingResult parseWhileOperator(TokenProvider startToken)
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

ParsingResult parseVariableDefinition(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::VariableDefinition, startToken, {
        {parseType, parseIdentifier, makeOperatorTokenParser("="), parseExpression},
        {parseType, parseIdentifier}
    });
}

ParsingResult parseAssignment(TokenProvider startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, startToken, {
        parseIdentifier, makeOperatorTokenParser("="), parseExpression
    });
}

ParsingResult parseFunctionCall(TokenProvider startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, startToken, {
        parseIdentifier, makeBreaketParser("("), parseCallArgumentsList, makeBreaketParser(")")
    });
}

ParsingResult parseCallArgumentsList(TokenProvider startToken)
{
    return processSyntaxRule(SyntaxRule::CallArgumentsList, startToken, {
        {parseExpression, makePunctuationParser(","), parseCallArgumentsList},
        {parseExpression}
    });
}

ParsingResult parseExpression(TokenProvider startToken)
{
    return parseExpression1(startToken);
}

ParsingResult parseExpression1(TokenProvider startToken)
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

ParsingResult parseExpression2(TokenProvider startToken)
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
    TokenProvider provider = {tokens.begin(), tokens.end()};
    auto [result, endTokentIt] = parseProgramm(provider);
    if (result.has_value())
    {
        return result.value();
    }
    else
    {
        throw std::exception();
    }
}