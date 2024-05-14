#include "INode.h"

std::string getTokenValue(const SyntaxTree & node)
{
    return node.tokens[0].value;
}