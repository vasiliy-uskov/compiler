#pragma once
#include <list>
#include "../tokenizer/Token.h"
#include "SyntaxTree.cpp"

std::pair<SyntaxTree, std::list<Token>::iterator> parserProgramm(std::list<Token>::iterator currentToken)
{
    auto functionsList = parserFunctionsList(currentToken);
    auto mainBody = parserFunctionsList(functionsList.second);
    return {
        SyntaxTree({functionsList.first, mainBody.first}),
        mainBody.second,
    };
}

std::pair<SyntaxTree, std::list<Token>::iterator> parserFunctionsList(std::list<Token>::iterator currentToken)
{
    
}

std::pair<SyntaxTree, std::list<Token>::iterator> parserMainBody(std::list<Token>::iterator currentToken)
{
    
}

class Parser
{
public:
    Parser(/* args */)
    {

    }
};