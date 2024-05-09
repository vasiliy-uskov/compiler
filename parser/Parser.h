#pragma once
#include <list>
#include <functional>
#include <optional>
#include <algorithm>
#include "SyntaxTree.h"
#include "ParserEngin.h"
#include "Grammar.h"

class Parser
{
private:
    static SyntaxTree deleteExtraNodes(SyntaxTree&);
public:
    static SyntaxTree parse(std::list<Token>);
};