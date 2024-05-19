#include "OperatorReturn.h"

OperatorReturn::OperatorReturn(const std::string & returnType, const SyntaxTree & _node)
    : node(_node),
    expectedReturnType(returnType),
    expression(ExpressionFactory::create(_node.children[1])) {}

TypeCheckErrors OperatorReturn::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return expression->initIdentifiersScope(_scope);
}
TypeCheckErrors OperatorReturn::checkTypes() const
{
    TypeCheckErrors errors;
    try
    {
        auto realType = expression->getType();
        if (realType != expectedReturnType)
        {   
            errors.add(InvalidTypeException(expression->getNode(), realType, expectedReturnType));
        }
    }
    catch (TypeCheckErrors e)
    {
        errors.add(e);
    }
    return errors;
}