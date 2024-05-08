#include "Grammar.h"

std::ostream& operator<<(std::ostream& os, const SyntaxRule type)
{
    switch(type)
    {
        case SyntaxRule::Program:
            return os << std::string("Program");
        case SyntaxRule::MainBody:
            return os << std::string("MainBody");
        case SyntaxRule::FunctionsList:
            return os << std::string("FunctionsList");
        case SyntaxRule::Function:
            return os << std::string("Function");
        case SyntaxRule::DefinitionArgumentsList:
            return os << std::string("DefinitionArgumentsList");
        case SyntaxRule::DefinitionArgument:
            return os << std::string("DefinitionArgument");
        case SyntaxRule::CallArgumentsList:
            return os << std::string("CallArgumentsList");
        case SyntaxRule::CallArgument:
            return os << std::string("CallArgument");
        case SyntaxRule::FunctionOperatorsList:
            return os << std::string("FunctionOperatorsList");
        case SyntaxRule::FunctionOperator:
            return os << std::string("FunctionOperator");
        case SyntaxRule::OperatorsList:
            return os << std::string("OperatorsList");
        case SyntaxRule::Operator:
            return os << std::string("Operator");
        case SyntaxRule::OperatorIf:
            return os << std::string("OperatorIf");
        case SyntaxRule::OperatorWhile:
            return os << std::string("OperatorWhile");
        case SyntaxRule::OperatorReturn:
            return os << std::string("OperatorReturn");
        case SyntaxRule::VariableDefinition:
            return os << std::string("VariableDefinition");
        case SyntaxRule::AssignmentOperator:
            return os << std::string("AssignmentOperator");
        case SyntaxRule::FunctionCall:
            return os << std::string("FunctionCall");
        case SyntaxRule::Expression:
            return os << std::string("Expression");
        case SyntaxRule::Breaket:
            return os << std::string("Breaket");
        case SyntaxRule::Punctuation:
            return os << std::string("Punctuation");
        case SyntaxRule::Keyword:
            return os << std::string("Keyword");
        case SyntaxRule::Identifier:
            return os << std::string("Identifier");
        case SyntaxRule::IntValue:
            return os << std::string("IntValue");
        case SyntaxRule::FloatValue:
            return os << std::string("FloatValue");
        case SyntaxRule::Type:
            return os << std::string("Type");
    }

    return os;
}

ParsingResult<SyntaxRule> parseProgramm(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Program, it, {
        {parseFunctionsList, parseMainBody},
        {parseMainBody}
    });
}

ParsingResult<SyntaxRule> parseFunctionsList(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionsList, it, {
        {parseFunction, parseFunctionsList},
        {parseFunction}
    });
}

ParsingResult<SyntaxRule> parseFunction(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParseFn<SyntaxRule> makeBreaketParser(ParserEngin<SyntaxRule> & engin, const std::string & breaket)
{
    return engin.makeTokenParserByValue(SyntaxRule::Breaket, breaket);
}

ParseFn<SyntaxRule> makePunctuationParser(ParserEngin<SyntaxRule> & engin, const std::string & punctuation)
{
    return engin.makeTokenParserByValue(SyntaxRule::Punctuation, punctuation);
}

ParseFn<SyntaxRule> makeKeywordParser(ParserEngin<SyntaxRule> & engin, const std::string & keyword)
{
    return engin.makeTokenParserByValue(SyntaxRule::Keyword, keyword);
}

ParseFn<SyntaxRule> makeOperatorTokenParser(ParserEngin<SyntaxRule> & engin, const std::string & operatorTokenValue)
{
    return engin.makeTokenParserByValue(SyntaxRule::Keyword, operatorTokenValue);
}

ParsingResult<SyntaxRule> parseType(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::Type, it, {
        {makeKeywordParser(engin, "int")},
        {makeKeywordParser(engin, "float")},
        {makeKeywordParser(engin, "bool")}
    });
}

ParsingResult<SyntaxRule> parseIdentifier(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::Identifier, TokenType::Identifier)(engin, it);
}

ParsingResult<SyntaxRule> parseIntValue(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::IntValue, TokenType::IntValue)(engin, it);
}

ParsingResult<SyntaxRule> parseFloatValue(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.makeTokenParserByType(SyntaxRule::FloatValue, TokenType::FloatValue)(engin, it);
}


ParsingResult<SyntaxRule> parseDefinitionArgumentsList(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::DefinitionArgumentsList, it, {
        {parseDefinitionArgument, makePunctuationParser(engin, ","), parseDefinitionArgumentsList},
        {parseDefinitionArgument}
    });
}

ParsingResult<SyntaxRule> parseDefinitionArgument(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::DefinitionArgumentsList, it, {
        {parseType, parseIdentifier}
    });
}

ParsingResult<SyntaxRule> parseMainBody(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseFunctionOperatorsList(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::FunctionOperatorsList, it, {
        {parseFunctionOperator, parseFunctionOperatorsList},
        {parseFunctionOperator}
    });
}

ParsingResult<SyntaxRule> parseFunctionOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseFunctionIfOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseFunctionWhileOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseReturnOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorReturn, it, {{
        makeKeywordParser(engin, "return"),
        parseExpression,
    }});
}

ParsingResult<SyntaxRule> parseOperatorsList(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::OperatorsList, it, {
        {parseOperator, parseOperatorsList},
        {parseOperator}
    });
}

ParsingResult<SyntaxRule> parseOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseIfOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseWhileOperator(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseVariableDefinition(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::VariableDefinition, it, {
        {parseType, parseIdentifier, makeOperatorTokenParser(engin, "="), parseExpression},
        {parseType, parseIdentifier}
    });
}

ParsingResult<SyntaxRule> parseAssignment(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::AssignmentOperator, it, {
        {parseIdentifier, makeOperatorTokenParser(engin, "="), parseExpression}
    });
}

ParsingResult<SyntaxRule> parseFunctionCall(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::AssignmentOperator, it, {
        {parseIdentifier, makeBreaketParser(engin, "("), parseCallArgumentsList, makeBreaketParser(engin, ")")}
    });
}

ParsingResult<SyntaxRule> parseCallArgumentsList(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return engin.processSyntaxRule(SyntaxRule::CallArgumentsList, it, {
        {parseExpression, makePunctuationParser(engin, ","), parseCallArgumentsList},
        {parseExpression}
    });
}

ParsingResult<SyntaxRule> parseExpression(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
{
    return parseExpression1(engin, it);
}

ParsingResult<SyntaxRule> parseExpression1(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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

ParsingResult<SyntaxRule> parseExpression2(ParserEngin<SyntaxRule> & engin, const TokenIterator & it)
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