#include "IdentifiersScope.h"

IdentifiersScope::IdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope) noexcept
    :parentScope(parentScope) {}


template <typename T>
void add(std::map<std::string, T> & map, const std::string & identifier, const T & type)
{
    if (map.count(identifier))
    {
        throw IdentifiersScope::DublicateException();
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
        if (parentScope)
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
        if (parentScope)
        {
            return parentScope->getVariableType(identifier);
        }
        throw UnknownIdentifierException();
    }
}