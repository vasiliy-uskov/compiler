#pragma once
#include <list>
#include <functional>
#include <optional>
#include <algorithm>
#include "JSLikeVectorAdapter.h"
#include "Grammar.h"

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};