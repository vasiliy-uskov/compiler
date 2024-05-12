#include "Token.h"

UnknownOperatorExeption::UnknownOperatorExeption(const std::string & op, const TokenPosition & position)
    : _operator(op), _position(position) {}
    
const char* UnknownOperatorExeption::what() const noexcept
{
    std::ostringstream message;
    message << "Unknown symbol(" << _position.line << "," <<  _position.end - _operator.size() << "): " << _operator;
    return std::string(message.str()).c_str();
}

const std::array<std::string, 14> definedOperators = {
    "||", "&&", "==", ">=", "<=", "<", ">", "*", "/", "+", "-", "=", "!", "!="
};

const std::array<std::string, 11> definedKeywords = {
    "false", "true", "int", "float", "bool", "return", "void", "while", "if", "else", "main"
};

template <typename T, std::size_t _Num>
bool inArr(const std::array<T, _Num> & arr, const T & value)
{
    return std::any_of(
        arr.begin(),
        arr.end(),
        [value] (const std::string op) {
            return op == value;
        }
    );
}

bool isOperatorExist(const std::string & _operator)
{
    return inArr(definedOperators, _operator);
}

Token::Token(const TokenType & type, const std::string & value, const TokenPosition & position)
        : type(type), value(value), position(position)
{
    if (type == TokenType::Operator && !isOperatorExist(value))
    {
        throw UnknownOperatorExeption(value, position);
    }
}

    
bool Token::isKeyword(const std::string & keyword)
{
    return inArr(definedKeywords, keyword);
}


std::ostream& operator<<(std::ostream& os, TokenPosition position)
{
    return os << "(" << position.line << "," <<  position.end << ")";   
}

std::ostream& operator<<(std::ostream& os, const TokenType type)
{
    return os << fromTokenTypeToString(type);
}

std::string fromTokenTypeToString(TokenType type)
{
    switch(type)
    {
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Keyword:
            return "Keyword";
        case TokenType::IntValue:
            return "IntValue";
        case TokenType::FloatValue:
            return "FloatValue";
        case TokenType::Punctuation:
            return "Punctuation";
        case TokenType::Breaket:
            return "Breaket";
        case TokenType::Operator:
            return "Operator";
    }   
    throw std::exception();
}