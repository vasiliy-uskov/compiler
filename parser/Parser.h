#pragma once
#include <list>
#include "Grammar.h"
#include "ParserEngin.h"

class Parser
{
public:
    static SyntaxTree<SyntaxRule> parse(std::list<Token>);
};