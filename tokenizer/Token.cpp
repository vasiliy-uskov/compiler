#pragma once
#include <string>
#include <array>
#include <sstream>
#include <algorithm>
#include "Token.h"

UnknownOperatorExeption::UnknownOperatorExeption(const std::string & op, const TokenPosition & position)
    : _operator(op), _position(position) {}
    
const char* UnknownOperatorExeption::what() const noexcept
{
    std::ostringstream message;
    message << "Unknown symbol(" << _position.line << "," <<  _position.end - _operator.size() << "): " << _operator;
    return std::string(message.str()).c_str();
}

std::array<std::string, 12> definedOperators = {
    "||", "&&", "==", ">=", "<=", "<", ">", "*", "/", "+", "-", "="
};

std::array<std::string, 11> definedKeywords = {
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

std::ostream& operator<<(std::ostream& os, const TokenType type)
{
    switch(type)
    {
        case TokenType::Identifier:
            return os << std::string("Identifier");
        case TokenType::Keyword:
            return os << std::string("Keyword");
        case TokenType::IntValue:
            return os << std::string("IntValue");
        case TokenType::FloatValue:
            return os << std::string("FloatValue");
        case TokenType::Punctuation:
            return os << std::string("Punctuation");
        case TokenType::Breaket:
            return os << std::string("Breaket");
        case TokenType::Operator:
            return os << std::string("Operator");
    }

    return os;
}