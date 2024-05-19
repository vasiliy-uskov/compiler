#include "FunctionDefinition.h"

FunctionDefinitionArgument::FunctionDefinitionArgument(const SyntaxTree & argumentNode)
    : node(argumentNode), 
    type(getTokenValue(argumentNode.children[0])),
    identifier(getTokenValue(argumentNode.children[1])) {}

FunctionDefinition::FunctionDefinition(const SyntaxTree & functionNode)
    : node(functionNode), 
    returnType(getTokenValue(functionNode.children[0])),
    identifier(getTokenValue(functionNode.children[1])),
    arguments(functionNode.children[2].rule == SyntaxRule::DefinitionArgumentsList
        ? buildList<FunctionDefinitionArgument>(functionNode.children[2], [](auto node) {
            return FunctionDefinitionArgument(node);
        })
        : std::vector<FunctionDefinitionArgument>({})),
    operatorsList(OperatorFactory::createFunctionOperatorsList(
        getTokenValue(functionNode.children[0]),
        functionNode.children[2].rule == SyntaxRule::DefinitionArgumentsList 
            ? functionNode.children[3]
            : functionNode.children[2]
    )) {}

TypeCheckErrors FunctionDefinition::checkTypes() const
{
    TypeCheckErrors errors;
    
    for (const IOperator::OperatorPtr & functionNode : operatorsList)
    {
        errors.add(functionNode->checkTypes());
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

    try
    {
        parentScope->addFunction(identifier, {returnType, argumentsTypes});
    }
    catch (IdentifiersScope::DublicateException & e)
    {
        errors.add(TypeCheckErrors({DublicateDeclorationException(node)}));
    }

    for (IOperator::OperatorPtr & oper : operatorsList)
    {
        errors.add(oper->initIdentifiersScope(scope));
    }
    return errors;
}