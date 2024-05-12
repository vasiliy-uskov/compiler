#include "IdentifiersScope.h"

IdentifiersScope::IdentifiersScope(IdentifiersScope & parentScope) noexcept
    :parentScope(std::optional<IdentifiersScope>(parentScope)), functions({}), variables({}) {}


template <typename T>
void add(std::map<std::string, T> & map, const std::string & identifier, const T & type)
{
    if (map.count(identifier))
    {
        throw DublicateDefinitionException();
    }
    map.insert({identifier, type});
}

void IdentifiersScope::addVariable(const std::string & identifier, const std::string & type)
{
    add(variables, identifier, type);
}
void IdentifiersScope::addFunction(const std::string & identifier, const FunctionSignature & type)
{
    add(functions, identifier, type);
}

FunctionSignature IdentifiersScope::getFunctionSignature(const std::string & identifier) const
{
    try
    {
        return functions.at(identifier);
    }
    catch (std::out_of_range&)
    {
        if (parentScope.has_value())
        {
            return parentScope->getFunctionSignature(identifier);
        }
        throw UnknownIdentifierException();
    }
}
std::string IdentifiersScope::getVariableType(const std::string & identifier) const
{
    try
    {
        return variables.at(identifier);
    }
    catch (std::out_of_range &)
    {
        if (parentScope.has_value())
        {
            return parentScope->getVariableType(identifier);
        }
        throw UnknownIdentifierException();
    }
}