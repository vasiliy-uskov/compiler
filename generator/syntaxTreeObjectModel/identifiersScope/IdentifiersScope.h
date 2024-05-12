#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>

typedef std::pair<std::string, std::vector<std::string>> FunctionSignature;
class IdentifiersScope;

class IdentifiersScope
{
private:
    std::shared_ptr<IdentifiersScope> parentScope = nullptr;
    std::map<std::string, std::string> variables;
    std::map<std::string, FunctionSignature> functions;
public:
    IdentifiersScope(const std::shared_ptr<IdentifiersScope> &) noexcept;
    void addVariable(const std::string & identifier, const std::string & type);
    void addFunction(const std::string & identifier, const FunctionSignature & signature);
    FunctionSignature getFunctionSignature(const std::string & identifier) const;
    std::string getVariableType(const std::string & identifier) const;

    class DublicateException {};
    class UnknownIdentifierException {};
};
