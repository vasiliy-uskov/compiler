#include "Grammar.h"

ParsingResult parseProgramm(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Program, it, {
        {parseFunctionsList, parseMainBody},
        {parseMainBody}
    });
}

ParsingResult parseFunctionsList(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionsList, it, {
        {parseFunction, parseFunctionsList},
        {parseFunction}
    });
}

ParsingResult parseFunction(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Function, it, {
        {
            parseType,
            parseIdentifier,
            makeBreaketParser(engin, "("),
            parseDefinitionArgumentsList,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseFunctionOperatorsList,
            makeBreaketParser(engin, "}")
        }, {
            makeKeywordParser(engin, "void"),
            parseIdentifier,
            makeBreaketParser(engin, "("),
            parseDefinitionArgumentsList,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseOperatorsList,
            makeBreaketParser(engin, "}")
        },
    });
}

ParseFn makeBreaketParser(ParserEngin & engin, const std::string & breaket)
{
    return engin.makeTokenParserByValue(SyntaxRule::Breaket, breaket);
}

ParseFn makePunctuationParser(ParserEngin & engin, const std::string & punctuation)
{
    return engin.makeTokenParserByValue(SyntaxRule::Punctuation, punctuation);
}

ParseFn makeKeywordParser(ParserEngin & engin, const std::string & keyword)
{
    return engin.makeTokenParserByValue(SyntaxRule::Keyword, keyword);
}

ParseFn makeOperatorTokenParser(ParserEngin & engin, const std::string & operatorTokenValue)
{
    return engin.makeTokenParserByValue(SyntaxRule::Keyword, operatorTokenValue);
}

ParsingResult parseType(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Type, it, {
        {makeKeywordParser(engin, "int")},
        {makeKeywordParser(engin, "float")},
        {makeKeywordParser(engin, "bool")}
    });
}

ParsingResult parseIdentifier(ParserEngin & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::Identifier, TokenType::Identifier)(engin, it);
}

ParsingResult parseIntValue(ParserEngin & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::IntValue, TokenType::IntValue)(engin, it);
}

ParsingResult parseFloatValue(ParserEngin & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::FloatValue, TokenType::FloatValue)(engin, it);
}


ParsingResult parseDefinitionArgumentsList(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::DefinitionArgumentsList, it, {
        {parseDefinitionArgument, makePunctuationParser(engin, ","), parseDefinitionArgumentsList},
        {parseDefinitionArgument}
    });
}

ParsingResult parseDefinitionArgument(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::DefinitionArgumentsList, it, {
        {parseType, parseIdentifier}
    });
}

ParsingResult parseMainBody(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::MainBody, it, {
        {
            makeKeywordParser(engin, "main"),
            makeBreaketParser(engin, "{"),
            parseOperatorsList,
            makeBreaketParser(engin, "}")
        },
        {
            makeKeywordParser(engin, "main"),
            makeBreaketParser(engin, "{"),
            makeBreaketParser(engin, "}")
        }
    });
}

ParsingResult parseFunctionOperatorsList(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionOperatorsList, it, {
        {parseFunctionOperator, parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult parseFunctionOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionOperator, it, {
        {parseVariableDefinition, makePunctuationParser(engin, ";")},
        {parseAssignment, makePunctuationParser(engin, ";")},
        {parseFunctionCall, makePunctuationParser(engin, ";")},
        {parseFunctionIfOperator},
        {parseFunctionWhileOperator},
        {parseReturnOperator, makePunctuationParser(engin, ";")},
        {makePunctuationParser(engin, ";")}
    });
}

ParsingResult parseFunctionIfOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorIf, it, {
        {
            makeKeywordParser(engin, "if"),
            makeBreaketParser(engin, "("),
            parseExpression,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseFunctionOperatorsList,
            makeBreaketParser(engin, "}"),
            makeKeywordParser(engin, "else"),
            makeBreaketParser(engin, "{"),
            parseFunctionOperatorsList,
            makeBreaketParser(engin, "}")
        },
        {
            makeKeywordParser(engin, "if"),
            makeBreaketParser(engin, "("),
            parseExpression,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseFunctionOperatorsList,
            makeBreaketParser(engin, "}"),
        }
    });
   
}

ParsingResult parseFunctionWhileOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorWhile, it, {{
        makeKeywordParser(engin, "while"),
        makeBreaketParser(engin, "("),
        parseExpression,
        makeBreaketParser(engin, ")"),
        makeBreaketParser(engin, "{"),
        parseFunctionOperatorsList,
        makeBreaketParser(engin, "}")
    }});
}

ParsingResult parseReturnOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorReturn, it, {{
        makeKeywordParser(engin, "return"),
        parseExpression,
    }});
}

ParsingResult parseOperatorsList(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorsList, it, {
        {parseOperator, parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult parseOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Operator, it, {
        {parseVariableDefinition, makePunctuationParser(engin, ";")},
        {parseAssignment, makePunctuationParser(engin, ";")},
        {parseFunctionCall, makePunctuationParser(engin, ";")},
        {parseIfOperator},
        {parseWhileOperator},
        {makePunctuationParser(engin, ";")}
    });
}

ParsingResult parseIfOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorIf, it, {
        {
            makeKeywordParser(engin, "if"),
            makeBreaketParser(engin, "("),
            parseExpression,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseOperatorsList,
            makeBreaketParser(engin, "}"),
            makeKeywordParser(engin, "else"),
            makeBreaketParser(engin, "{"),
            parseOperatorsList,
            makeBreaketParser(engin, "}")
        },
        {
            makeKeywordParser(engin, "if"),
            makeBreaketParser(engin, "("),
            parseExpression,
            makeBreaketParser(engin, ")"),
            makeBreaketParser(engin, "{"),
            parseOperatorsList,
            makeBreaketParser(engin, "}")
        }
    });
   
}

ParsingResult parseWhileOperator(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorWhile, it, {{
        makeKeywordParser(engin, "while"),
        makeBreaketParser(engin, "("),
        parseExpression,
        makeBreaketParser(engin, ")"),
        makeBreaketParser(engin, "{"),
        parseOperatorsList,
        makeBreaketParser(engin, "}")
    }});
}

ParsingResult parseVariableDefinition(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::VariableDefinition, it, {
        {parseType, parseIdentifier, makeOperatorTokenParser(engin, "="), parseExpression},
        {parseType, parseIdentifier}
    });
}

ParsingResult parseAssignment(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::AssignmentOperator, it, {
        {parseIdentifier, makeOperatorTokenParser(engin, "="), parseExpression}
    });
}

ParsingResult parseFunctionCall(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionCall, it, {
        {parseIdentifier, makeBreaketParser(engin, "("), parseCallArgumentsList, makeBreaketParser(engin, ")")}
    });
}

ParsingResult parseCallArgumentsList(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::CallArgumentsList, it, {
        {parseExpression, makePunctuationParser(engin, ","), parseCallArgumentsList},
        {parseExpression}
    });
}

ParsingResult parseExpression(ParserEngin & engin, const TokenIterator & it)
{
    return parseExpression1(engin, it);
}

ParsingResult parseExpression1(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Expression, it, {
        {parseExpression2, makeOperatorTokenParser(engin, "||"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "+"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "-"), parseExpression1},
        {makeOperatorTokenParser(engin, "-"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "&&"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "*"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "/"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "=="), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "<"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, "<="), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, ">"), parseExpression1},
        {parseExpression2, makeOperatorTokenParser(engin, ">="), parseExpression1},
        {parseExpression2}
    });    
}

ParsingResult parseExpression2(ParserEngin & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Expression, it, {
        {makeKeywordParser(engin, "true")},
        {makeKeywordParser(engin, "false")},
        {parseIntValue},
        {parseFloatValue},
        {parseFunctionCall},
        {parseIdentifier},
        {makeBreaketParser(engin, "("), parseExpression1, makeBreaketParser(engin, ")")}
    });  
}