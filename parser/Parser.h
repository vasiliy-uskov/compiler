#pragma once
#include <list>
#include <functional>
#include <optional>
#include "SyntaxTree.h"
#include "ParserEngin.h"
#include "Grammar.h"

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};