#include "AssignmentOperator.h"

AssignmentOperator::AssignmentOperator(const SyntaxTree & node)
    : node(node),
    variable(Variable(node.children[0])),
    expression(ExpressionFactory::create(node.children[2])) {}

TypeCheckErrors AssignmentOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    auto errors = variable.initIdentifiersScope(_scope);
    errors.add(expression->initIdentifiersScope(_scope));
    return {};
}
TypeCheckErrors AssignmentOperator::checkTypes() const
{
    TypeCheckErrors errors;
    try
    {
        auto variableType = variable.getType();
        auto expressionType = expression->getType();
        if (variableType != expressionType
            && !(variableType == "flaot" && expressionType == "int"))
        {
            return TypeCheckErrors({InvalidTypeException(expression->getNode(), expressionType, variableType)});
        }
    }
    catch(TypeCheckErrors e)
    {
        return e;
    }
    
    return errors;
}