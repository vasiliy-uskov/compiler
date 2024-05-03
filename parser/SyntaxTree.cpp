#pragma once
#include "SyntaxTree.h"


SyntaxTree::SyntaxTree(SyntaxRule rule, const std::list<Token> & tokens)
        : rule(rule), tokens(tokens), children({}) {}
SyntaxTree::SyntaxTree(SyntaxRule rule, const std::list<Token> & tokens, const std::vector<SyntaxTree> & _children)
        : rule(rule), tokens(tokens), children(_children) {}