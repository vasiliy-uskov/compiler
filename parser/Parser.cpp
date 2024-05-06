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
        if (ruleResult.has_value())
        {
            children.push_back(ruleResult.value());
            currentToken = ruleEndToken;
        }
        else
        {
            return {std::nullopt, currentToken};
        }
    }
    return {
        SyntaxTree(rule, {startToken, currentToken}, children),
        currentToken
    };
}

ParsingResult processSyntaxRule(SyntaxRule rule, const TokenIterator & startToken, const std::list<std::list<ParseFn>> & ruleVariants)
{
    for (auto variant = ruleVariants.begin(); variant != ruleVariants.end(); variant++)
    {
        auto [ruleResult, ruleEndToken] = processSyntaxRuleVariant(rule, startToken, *variant);
        if (ruleResult.has_value())
        {
            return {
                ruleResult,
                ruleEndToken
            };
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
            makePunctuationParser(";"),
            makeBreaketParser("}")
        }, {
            makeKeywordParser("void"),
            parseIdentifier,
            makeBreaketParser("("),
            parseDefinitionArgumentsList,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makePunctuationParser(";"),
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
            makePunctuationParser(";"),
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
        {parseFunctionOperator, makePunctuationParser(";"), parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult parseFunctionOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::FunctionOperator, startToken, {
        {parseVariableDefinition},
        {parseAssignment},
        {parseFunctionCall},
        {parseFunctionIfOperator},
        {parseFunctionWhileOperator},
        {parseReturnOperator}
    });
}

ParsingResult parseFunctionIfOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, startToken, {
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseLogicalExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}"),
            makeKeywordParser("else"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}")
        },
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseLogicalExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseFunctionOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}"),
        }
    });
   
}

ParsingResult parseFunctionWhileOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, startToken, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseLogicalExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseFunctionOperatorsList,
        makePunctuationParser(";"),
        makeBreaketParser("}")
    });
}

ParsingResult parseReturnOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorReturn, startToken, {
        makeKeywordParser("return"),
        parseExpression
    });
}

ParsingResult parseOperatorsList(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorsList, startToken, {
        {parseOperator, makePunctuationParser(";"), parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult parseOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::Operator, startToken, {
        {parseVariableDefinition},
        {parseAssignment},
        {parseFunctionCall},
        {parseIfOperator},
        {parseWhileOperator}
    });
}

ParsingResult parseIfOperator(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::OperatorIf, startToken, {
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseLogicalExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}"),
            makeKeywordParser("else"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}")
        },
        {
            makeKeywordParser("if"),
            makeBreaketParser("("),
            parseLogicalExpression,
            makeBreaketParser(")"),
            makeBreaketParser("{"),
            parseOperatorsList,
            makePunctuationParser(";"),
            makeBreaketParser("}")
        }
    });
   
}

ParsingResult parseWhileOperator(TokenIterator startToken)
{
    return processSyntaxRuleVariant(SyntaxRule::OperatorWhile, startToken, {
        makeKeywordParser("while"),
        makeBreaketParser("("),
        parseLogicalExpression,
        makeBreaketParser(")"),
        makeBreaketParser("{"),
        parseOperatorsList,
        makePunctuationParser(";"),
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
    return processSyntaxRule(SyntaxRule::Expression, startToken, {
        {parseArithmeticExpression},
        {parseLogicalExpression}
    });    
}

ParsingResult parseLogicalExpression(TokenIterator startToken)
{
    return parseLogicalExpression1(startToken);
}

ParsingResult parseLogicalExpression1(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseLogicalExpression2, makeOperatorTokenParser("||"), parseLogicalExpression1},
        {parseLogicalExpression2}
    });    
}

ParsingResult parseLogicalExpression2(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseLogicalExpression3, makeOperatorTokenParser("&&"), parseLogicalExpression2},
        {parseLogicalExpression3},
    });    
}

ParsingResult parseLogicalExpression3(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseArithmeticExpression, makeOperatorTokenParser("=="), parseArithmeticExpression},
        {parseArithmeticExpression, makeOperatorTokenParser("<"), parseArithmeticExpression},
        {parseArithmeticExpression, makeOperatorTokenParser("<="), parseArithmeticExpression},
        {parseArithmeticExpression, makeOperatorTokenParser(">"), parseArithmeticExpression},
        {parseArithmeticExpression, makeOperatorTokenParser(">="), parseArithmeticExpression},
        {parseFunctionCall},
        {parseIdentifier},
        {makeKeywordParser("true")},
        {makeKeywordParser("false")},
        {makeBreaketParser("("), parseLogicalExpression1, makeBreaketParser(")")}
    });    
}

ParsingResult parseArithmeticExpression(TokenIterator startToken)
{
    return parseArithmeticExpression1(startToken);
}

ParsingResult parseArithmeticExpression1(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseArithmeticExpression2, makeOperatorTokenParser("+"), parseArithmeticExpression1},
        {parseArithmeticExpression2, makeOperatorTokenParser("-"), parseArithmeticExpression1},
        {makeOperatorTokenParser("-"), parseArithmeticExpression1},
        {parseArithmeticExpression2},
    });    
}

ParsingResult parseArithmeticExpression2(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseArithmeticExpression3, makeOperatorTokenParser("*"), parseArithmeticExpression2},
        {parseArithmeticExpression3, makeOperatorTokenParser("/"), parseArithmeticExpression2},
        {parseArithmeticExpression3}
    });    
}

ParsingResult parseArithmeticExpression3(TokenIterator startToken)
{
    return processSyntaxRule(SyntaxRule::ArithmeticExpression, startToken, {
        {parseFunctionCall},
        {parseIdentifier},
        {parseIntValue},
        {parseFloatValue},
        {makeBreaketParser("("), parseArithmeticExpression1, makeBreaketParser(")")}
    });    
}

SyntaxTree Parser::parse(std::list<Token> tokens)
{
    auto [SyntaxTreeOptional, endTokentIt] = parseProgramm(tokens.begin());
    if (SyntaxTreeOptional.has_value())
    {
        return SyntaxTreeOptional.value();
    }
    else
    {
        throw std::exception();
    }
}