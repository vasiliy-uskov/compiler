#include "FunctionDefinition.h"

FunctionDefinitionArgument::FunctionDefinitionArgument(const SyntaxTree & argumentNode)
    : node(argumentNode), 
    type(getTokenValue(argumentNode.children[0])),
    identifier(getTokenValue(argumentNode.children[1])) {}

FunctionDefinition::FunctionDefinition(const SyntaxTree & functionNode)
    : node(functionNode), 
    returnType(getTokenValue(node.children[0])),
    identifier(getTokenValue(node.children[1])),
    arguments(buildList<FunctionDefinitionArgument>(node.children[2], [](auto node) {
        return FunctionDefinitionArgument(node);
    })),
    operatorsList(OperatorFactory::createFunctionOperatorsList(returnType, node.children[3])) {}

TypeCheckErrors FunctionDefinition::checkTypes() const
{
    TypeCheckErrors errors;
    
    for (const IOperator & node : operatorsList)
    {
        errors.add(node.checkTypes());
    };

    return errors;
}

TypeCheckErrors FunctionDefinition::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope)
{
    scope = std::make_shared<IdentifiersScope>(parentScope);
    TypeCheckErrors errors;
    std::vector<std::string> argumentsTypes;

    for (const FunctionDefinitionArgument & argument : arguments)
    {
        argumentsTypes.push_back(argument.type);
        try
        {
            scope->addVariable(argument.identifier, argument.type);
        }
        catch (IdentifiersScope::DublicateException & e)
        {
            errors.add(TypeCheckErrors({DublicateDeclorationException(argument.node)}));
        }
    }

    for (IOperator & oper : operatorsList)
    {
        errors.add(oper.initIdentifiersScope(scope));
    }

    try
    {
        scope->addFunction(identifier, {returnType, argumentsTypes});
    }
    catch (IdentifiersScope::DublicateException & e)
    {
        errors.add(TypeCheckErrors({DublicateDeclorationException(node)}));
    }
    return errors;
}