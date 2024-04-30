#pragma once
#include <iostream>
#include <list>
#include <string>
#include "Token.cpp"

enum ParserState
{
    Start,
    Space,
    Identifier,
    IntValue,
    FloatValue,
    Punctuation,
    Breaket,
    Operator,
    Comment
};

class UnexpectedSymbolExeption: public std::exception
{
private:
    const char _symbol;
public:
    UnexpectedSymbolExeption(char symbol) : _symbol(symbol) {}
    
    const char* what() const noexcept override
    {
        return "Unexpected symbol: " + _symbol;
    }
};

class Parser
{
public:
    static std::list<Token> parse(std::istream & input)
    {
        std::list<Token> tokens = std::list<Token>();
        std::string tokenValue = "";
        ParserState parserState = ParserState::Start;
        char ch;
        while (input.get(ch)) 
        {
            if (parserState == ParserState::Comment)
            {
                if (ch == '\n')
                {
                    parserState = ParserState::Space;
                }
                continue;
            }
            if (isalpha(ch))
            {
                switch (parserState)
                {
                case ParserState::Start:
                case ParserState::Space:
                case ParserState::Identifier:
                    tokenValue += ch;
                    parserState = ParserState::Identifier;
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::Identifier;
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::Identifier;
                    break;
                case ParserState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::Identifier;
                    break;
                }
                continue;
            }
            if (isdigit(ch))
            {
                switch (parserState)
                {
                case ParserState::Start:
                case ParserState::Space:
                    tokenValue += ch;
                    parserState = ParserState::IntValue;
                    break;
                case ParserState::IntValue:
                case ParserState::FloatValue:
                case ParserState::Identifier:
                    tokenValue += ch;
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::IntValue;
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::IntValue;
                    break;
                case ParserState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    tokenValue = ch + "";
                    parserState = ParserState::IntValue;
                    break;
                }
                continue;
            }
            if (ch == ')'
                || ch == '('
                || ch == '{'
                || ch == '}')
            {
                switch (parserState)
                {
                case ParserState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case ParserState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case ParserState::Identifier:
                    if (Token::isKeyword(tokenValue))
                    {
                        tokens.push_front(Token(Token::TokenType::Keyword, tokenValue));
                    }
                    else
                    {
                        tokens.push_front(Token(Token::TokenType::Identifier, tokenValue));
                    }
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case ParserState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = ch + "";
                parserState = ParserState::Breaket;
                continue;
            }
            if (ch == ';'
                || ch == ',')
            {
                switch (parserState)
                {
                case ParserState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case ParserState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case ParserState::Identifier:
                    if (Token::isKeyword(tokenValue))
                    {
                        tokens.push_front(Token(Token::TokenType::Keyword, tokenValue));
                    }
                    else
                    {
                        tokens.push_front(Token(Token::TokenType::Identifier, tokenValue));
                    }
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case ParserState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = ch + "";
                parserState = ParserState::Punctuation;
                continue;
            }
            if (ch == '+'
                || ch == '-'
                || ch == '/'
                || ch == '*'
                || ch == '|'
                || ch == '&')
            {
                switch (parserState)
                {
                case ParserState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case ParserState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case ParserState::Identifier:
                    if (Token::isKeyword(tokenValue))
                    {
                        tokens.push_front(Token(Token::TokenType::Keyword, tokenValue));
                    }
                    else
                    {
                        tokens.push_front(Token(Token::TokenType::Identifier, tokenValue));
                    }
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case ParserState::Operator:
                    if (tokenValue == "/")
                    {
                        tokenValue = "";
                        parserState = ParserState::Comment;
                    }
                    else
                    {
                        tokenValue += ch;
                    }
                    break;
                }
                tokenValue = ch + "";
                parserState = ParserState::Operator;
                continue;
            }
            if (ch == '\n'
                || ch == ' '
                || ch == '\t')
            {
                switch (parserState)
                {
                case ParserState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case ParserState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case ParserState::Identifier:
                    if (Token::isKeyword(tokenValue))
                    {
                        tokens.push_front(Token(Token::TokenType::Keyword, tokenValue));
                    }
                    else
                    {
                        tokens.push_front(Token(Token::TokenType::Identifier, tokenValue));
                    }
                    break;
                case ParserState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case ParserState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case ParserState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = "";
                parserState = ParserState::Space;
                continue;
            }
            if (ch == '.' && parserState == ParserState::IntValue)
            {
                tokenValue += ch;
                parserState = ParserState::FloatValue;
                continue;
            }
            throw UnexpectedSymbolExeption(ch);
        }
        return tokens;
    }
};
 