#pragma once
#include "SyntaxTree.h"
#include "Parser.h"

ParsingResult processSyntaxRuleVariant(SyntaxRule rule, const TokenProvider & startProvider, const std::list<ParseFn> & rules)
{
    std::vector<SyntaxTree> children = {};
    TokenProvider currentProvider = startProvider;
    for (auto rule : rules)
    {
        auto [ruleResult, ruleEndProvider] = rule(currentProvider);
        if (!ruleResult.has_value())
        {
            return {std::nullopt, ruleEndProvider};
        }
        children.push_back(ruleResult.value());
        currentProvider = ruleEndProvider;
    }
    return {
        SyntaxTree(rule, {startProvider.getIt(), currentProvider.getIt()}, children),
        currentProvider
    };
}

ParsingResult processSyntaxRule(SyntaxRule rule, const TokenProvider & provider, const std::list<std::list<ParseFn>> & ruleVariants)
{
    auto errorTokenProvider = provider; 
    for (auto ruleVariant : ruleVariants)
    {
        auto result = processSyntaxRuleVariant(rule, provider, ruleVariant);
        if (result.first.has_value())
        {
            return result;
        }
        if (0 < std::distance(errorTokenProvider.getIt(), result.second.getIt()))
        {
            errorTokenProvider = result.second;
        }
    }
    return {std::nullopt, errorTokenProvider};
}

ParsingResult parseToken(SyntaxRule rule, const TokenProvider & provider, std::function<bool(const Token&)> matcher)
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

ParsingResult parseProgramm(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Program, provider, {
        {parseFunctionsList, parseMainBody},
        {parseMainBody}
    });
}

ParsingResult parseFunctionsList(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::FunctionsList, provider, {
        {parseFunction, parseFunctionsList},
        {parseFunction}
    });
}

ParsingResult parseFunction(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Function, provider, {
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

ParsingResult parseType(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Type, provider, {
        {makeKeywordParser("int")},
        {makeKeywordParser("float")},
        {makeKeywordParser("bool")}
    });
}

ParsingResult parseIdentifier(const TokenProvider & provider)
{
    return makeTokenParserByType(SyntaxRule::Identifier, TokenType::Identifier)(provider);
}

ParsingResult parseIntValue(const TokenProvider & provider)
{
    return makeTokenParserByType(SyntaxRule::IntValue, TokenType::IntValue)(provider);
}

ParsingResult parseFloatValue(const TokenProvider & provider)
{
    return makeTokenParserByType(SyntaxRule::FloatValue, TokenType::FloatValue)(provider);
}


ParsingResult parseDefinitionArgumentsList(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::DefinitionArgumentsList, provider, {
        {parseDefinitionArgument, makePunctuationParser(","), parseDefinitionArgumentsList},
        {parseDefinitionArgument}
    });
}

ParsingResult parseDefinitionArgument(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::DefinitionArgumentsList, provider, {
        parseType, parseIdentifier
    });
}

ParsingResult parseMainBody(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::MainBody, provider, {
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

ParsingResult parseFunctionOperatorsList(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::FunctionOperatorsList, provider, {
        {parseFunctionOperator, parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult parseFunctionOperator(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::FunctionOperator, provider, {
        {parseVariableDefinition, makePunctuationParser(";")},
        {parseAssignment, makePunctuationParser(";")},
        {parseFunctionCall, makePunctuationParser(";")},
        {parseFunctionIfOperator},
        {parseFunctionWhileOperator},
        {parseReturnOperator, makePunctuationParser(";")},
        {makePunctuationParser(";")}
    });
}

ParsingResult parseFunctionIfOperator(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, provider, {
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

ParsingResult parseFunctionWhileOperator(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, provider, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseFunctionOperatorsList,
        makeBreaketParser("}")
    });
}

ParsingResult parseReturnOperator(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorReturn, provider, {
        makeKeywordParser("return"),
        parseExpression,
    });
}

ParsingResult parseOperatorsList(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::OperatorsList, provider, {
        {parseOperator, parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult parseOperator(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Operator, provider, {
        {parseVariableDefinition, makePunctuationParser(";")},
        {parseAssignment, makePunctuationParser(";")},
        {parseFunctionCall, makePunctuationParser(";")},
        {parseIfOperator},
        {parseWhileOperator},
        {makePunctuationParser(";")}
    });
}

ParsingResult parseIfOperator(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, provider, {
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

ParsingResult parseWhileOperator(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, provider, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseOperatorsList,
        makeBreaketParser("}")
    });
}

ParsingResult parseVariableDefinition(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::VariableDefinition, provider, {
        {parseType, parseIdentifier, makeOperatorTokenParser("="), parseExpression},
        {parseType, parseIdentifier}
    });
}

ParsingResult parseAssignment(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, provider, {
        parseIdentifier, makeOperatorTokenParser("="), parseExpression
    });
}

ParsingResult parseFunctionCall(const TokenProvider & provider)
{
    return processSyntaxRuleVariant(SyntaxRule::AssignmentOperator, provider, {
        parseIdentifier, makeBreaketParser("("), parseCallArgumentsList, makeBreaketParser(")")
    });
}

ParsingResult parseCallArgumentsList(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::CallArgumentsList, provider, {
        {parseExpression, makePunctuationParser(","), parseCallArgumentsList},
        {parseExpression}
    });
}

ParsingResult parseExpression(const TokenProvider & provider)
{
    return parseExpression1(provider);
}

ParsingResult parseExpression1(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Expression, provider, {
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

ParsingResult parseExpression2(const TokenProvider & provider)
{
    return processSyntaxRule(SyntaxRule::Expression, provider, {
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
    const TokenProvider & provider = {tokens.begin(), tokens.end()};
    auto [result, endTokent] = parseProgramm(provider);
    if (result.has_value())
    {
        return result.value();
    }
    else
    {
        std::cout
            << endTokent.get().value << " "
            << endTokent.get().position.line << "," << endTokent.get().position.line
            << std::endl; 
        throw std::exception();
    }
}