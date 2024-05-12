#include <string>
#include <list>
#include "../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"

struct InvalidTypeException
{
    const SyntaxTree node;
    const std::string realType;
    const std::string expectingType;

    InvalidTypeException(const SyntaxTree & node, const std::string & realType, const std::string & expectingType)
        : node(node), realType(realType), expectingType(expectingType) {}
};
struct DublicateDeclorationException
{
    const SyntaxTree node;

    DublicateDeclorationException(const SyntaxTree & node)
        : node(node) {}
};
struct InvalidArgumentsCountExcpetion
{
    const SyntaxTree node;
    const size_t realCount;
    const size_t expectingCount;

    InvalidArgumentsCountExcpetion(const SyntaxTree & node, size_t realCount, size_t expectingCount)
        : node(node), realCount(realCount), expectingCount(expectingCount) {}
};

struct UnknownFunctionException
{
    const SyntaxTree node;

    UnknownFunctionException(const SyntaxTree & node): node(node) {}
};

struct UnknownVariableException
{
    const SyntaxTree node;

    UnknownVariableException(const SyntaxTree & node): node(node) {}
};

struct TypeCheckErrors
{
    std::list<InvalidTypeException> invalidTypeErrors; 
    std::list<InvalidArgumentsCountExcpetion> argumentsCountErrors;
    std::list<DublicateDeclorationException> dublicateDeclarations;
    std::list<UnknownFunctionException> unknownFunctionsErrors;
    std::list<UnknownVariableException> unknownVariablesErrors;

    TypeCheckErrors() {}

    TypeCheckErrors(const std::list<InvalidTypeException> & errors)
        : invalidTypeErrors(errors) {}
    TypeCheckErrors(const std::list<InvalidArgumentsCountExcpetion> & errors)
        : argumentsCountErrors(errors) {}
    TypeCheckErrors(const std::list<DublicateDeclorationException> & errors)
        : dublicateDeclarations(errors) {}    
    TypeCheckErrors(const std::list<UnknownFunctionException> & errors)
        : unknownFunctionsErrors(errors) {}
    TypeCheckErrors(const std::list<UnknownVariableException> & errors)
        : unknownVariablesErrors(errors) {}

    void add(const TypeCheckErrors & errors)
    {
        std::copy(errors.invalidTypeErrors.begin(), errors.invalidTypeErrors.end(), std::back_inserter(invalidTypeErrors));
        std::copy(errors.argumentsCountErrors.begin(), errors.argumentsCountErrors.end(), std::back_inserter(argumentsCountErrors));
        std::copy(errors.dublicateDeclarations.begin(), errors.dublicateDeclarations.end(), std::back_inserter(dublicateDeclarations));
        std::copy(errors.unknownFunctionsErrors.begin(), errors.unknownFunctionsErrors.end(), std::back_inserter(unknownFunctionsErrors));
        std::copy(errors.unknownVariablesErrors.begin(), errors.unknownVariablesErrors.end(), std::back_inserter(unknownVariablesErrors));
    }
    
    void add(const InvalidTypeException & error)
    {
        invalidTypeErrors.push_back(error);
    }
    
    void add(const UnknownFunctionException & error)
    {
        unknownFunctionsErrors.push_back(error);
    }
    
    void add(const DublicateDeclorationException & error)
    {
        dublicateDeclarations.push_back(error);
    }

    bool isEmpty()
    {
        return !invalidTypeErrors.size()
            && !dublicateDeclarations.size()
            && !argumentsCountErrors.size()
            && !unknownFunctionsErrors.size()
            && !unknownVariablesErrors.size();
    }
};