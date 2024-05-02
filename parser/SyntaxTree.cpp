#pragma once
#include <vector>
#include <optional>
#include "../tokenizer/Token.h"

class SyntaxTree
{
public:
    const std::vector<SyntaxTree> children;
    const std::optional<Token> token;

    SyntaxTree(const Token & token)
        : token(token), children({}) {}
    SyntaxTree(const std::vector<SyntaxTree> & children)
        : token(), children(children) {}
    SyntaxTree(const Token & token, const std::vector<SyntaxTree> & children)
        : token(token), children(children) {}
};