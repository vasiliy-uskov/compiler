#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <list>
#include "identifiersScope/IdentifiersScope.h"
#include "TypeCheckErrors.cpp"
#include "../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"

struct INode
{
    virtual TypeCheckErrors checkTypes() const { throw 0; }
    virtual TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) { throw 0; }
};

std::string getTokenValue(const SyntaxTree & node);