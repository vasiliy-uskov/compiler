#include "Programm.h"

Programm::Programm(const SyntaxTree & node)
    : functions(node.children.size() == 2
        ? std::optional(buildList<FunctionDefinition>(node.children[0], [](auto node) {
            return FunctionDefinition(node);
        }))
        : std::nullopt),
    operators(buildList<IOperator>(
        node.children[node.children.size() == 2 ? 1 : 0].children[0],
        creaetOperator)) {}