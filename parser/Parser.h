#pragma once
#include <list>
#include <functional>
#include <optional>
#include <algorithm>
#include <map>
#include "./utils/JSLikeVectorAdapter.h"
#include "./grammar/Grammar.h"

class Parser
{
public:
    static SyntaxTree parse(std::list<Token>);
};