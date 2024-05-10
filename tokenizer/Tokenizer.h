#pragma once
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include "Token.h"

class UnexpectedSymbolExeption: public std::exception
{
private:
    const char _symbol;
    const TokenPosition _position;
public:
    UnexpectedSymbolExeption(char symbol, const TokenPosition & position);
    const char* what() const noexcept override;
};

class Tokenizer
{
private:
    Tokenizer();
public:
    static std::list<Token> parse(std::istream & input);
};