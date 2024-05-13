#pragma once
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include "Token.h"

class UnexpectedSymbolExeption
{
private:
    const char _symbol;
    const TokenPosition _position;
public:
    UnexpectedSymbolExeption(char symbol, const TokenPosition & position);
};

class Tokenizer
{
private:
    Tokenizer();
public:
    static std::list<Token> parse(std::istream & input);
};