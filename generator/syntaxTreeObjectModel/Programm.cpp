#include "Programm.h"

Programm::Programm(const SyntaxTree & node)
    : functions(node.children.size() == 2
        ? std::optional(buildList<FunctionDefinition>(node.children[0], [](auto node) {
            return FunctionDefinition(node);
        }))
        : std::nullopt),
    operators(OperatorFactory::createList(node.children.size() == 2
        ? node.children[1].children[0]
        : node.children[1].children[0])) {}

        
TypeCheckErrors Programm::checkTypes()
{
    TypeCheckErrors errors;
    
    forEachNode([&errors](INode & node) {
        errors.add(node.checkTypes());
    });

    return errors;
}
        
TypeCheckErrors Programm::initIdentifiersScope()
{
    scope = std::make_shared<IdentifiersScope>(nullptr);
    TypeCheckErrors errors;
    
    forEachNode([&errors, this](INode & node) {
        errors.add(node.initIdentifiersScope(scope));
    });

    return errors;
}

void Programm::forEachNode(const std::function<void(INode&)> & predicate)
{
    std::list<InvalidTypeException> errors;
    if (functions.has_value())
    {
        for (FunctionDefinition & function : functions.value())
        {
            predicate(function);
        }
    }
    for (IOperator & _operator : operators)
    {
        predicate(_operator);
    }
}