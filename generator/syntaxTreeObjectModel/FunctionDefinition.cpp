#include "FunctionDefinition.h"

FunctionDefinitionArgument::FunctionDefinitionArgument(const SyntaxTree & argumentNode)
    : type(getTokenValue(argumentNode.children[0])),
    identifier(getTokenValue(argumentNode.children[1])) {}

FunctionDefinition::FunctionDefinition(const SyntaxTree & functionNode)
    : returnType(getTokenValue(functionNode.children[0])),
    identifier(getTokenValue(functionNode.children[1])),
    arguments(buildList<FunctionDefinitionArgument>(functionNode.children[2], [](auto node) {
        return FunctionDefinitionArgument(node);
    })),
    operatorsList(buildList<IOperator>(functionNode.children[3], creaetOperator)) {}