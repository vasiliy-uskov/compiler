#pragma once
#include <string>
#include <exception>
#include <array>
#include <sstream>
#include <algorithm>

struct TokenPosition
{
    const size_t line;
    const size_t end;
};

class UnknownOperatorExeption: public std::exception
{
private:
    const std::string _operator;
    const TokenPosition _position;
public:
    UnknownOperatorExeption(const std::string & op, const TokenPosition & position);
    const char* what() const noexcept override;
};

enum class TokenType
{
    Identifier,
    Keyword,
    IntValue,
    FloatValue,
    Punctuation,
    Breaket,
    Operator
};

struct Token
{
    const TokenType type;
    const std::string value;
    const TokenPosition position; 

    Token(const TokenType & type, const std::string & value, const TokenPosition & position);
    static bool isKeyword(const std::string & keyword);
};

std::ostream& operator<<(std::ostream& os, TokenType type);
std::string fromTokenTypeToString(TokenType);