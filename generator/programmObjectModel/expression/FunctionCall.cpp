#include "FunctionCall.h"

FunctionCall::FunctionCall(const SyntaxTree & _node)
    : BaseExpression(_node),
    identifier(getTokenValue(_node.children[0])),
    arguments(_node.children.size() == 2
        ? ExpressionFactory::createList(_node.children[1])
        : std::vector<IExpression::ExpressionPtr>({})) {}

    
TypeCheckErrors FunctionCall::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope)
{
    auto errors = BaseExpression::initIdentifiersScope(scope);
    for (auto argument : arguments)
    {
        errors.add(argument->initIdentifiersScope(scope));
    }
    return errors;
}
        
std::string FunctionCall::getType() const
{
    auto errors = checkTypes();
    if (errors.isEmpty())
    {
        try
        {    
            return scope->getFunctionSignature(identifier).first;
        }
        catch(IdentifiersScope::UnknownIdentifierException e)
        {
            errors.add(UnknownFunctionException(node));
        }
        
    }
    throw errors;
}

TypeCheckErrors FunctionCall::checkTypes() const
{
    try
    {
        auto expectingArguments = scope->getFunctionSignature(identifier).second;    
        auto argumentsCount = arguments.size();
        auto expectingArgumentsCount = expectingArguments.size();
        if (argumentsCount != expectingArgumentsCount)
        {
            return TypeCheckErrors({InvalidArgumentsCountExcpetion(node, argumentsCount, expectingArgumentsCount)});
        }
        TypeCheckErrors errors;
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            try
            {
                auto actualType = arguments[i]->getType();
                auto expectingType = expectingArguments[i];
                if (actualType != expectingType)
                {
                    errors.add(InvalidTypeException(node, actualType, expectingType));
                }
            }
            catch (const TypeCheckErrors & newErrors)
            {
                errors.add(newErrors);
            }
        }
        return errors;
    }
    catch(IdentifiersScope::UnknownIdentifierException e)
    {
        return TypeCheckErrors({UnknownFunctionException(node)});
    }
}