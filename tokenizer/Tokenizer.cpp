#pragma once
#include <iostream>
#include <list>
#include <string>
#include "Token.cpp"

enum TokenizerState
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

void processAlphabitCh(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    switch (parserState)
    {
    case TokenizerState::Start:
    case TokenizerState::Space:
    case TokenizerState::Identifier:
        tokenValue += ch;
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::Breaket:
        tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::Punctuation:
        tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::Operator:
        tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::IntValue:
    case TokenizerState::FloatValue:
        throw UnexpectedSymbolExeption(ch);
    }
}

void processDigit(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    switch (parserState)
    {
    case TokenizerState::Start:
    case TokenizerState::Space:
        tokenValue += ch;
        parserState = TokenizerState::IntValue;
        break;
    case TokenizerState::IntValue:
    case TokenizerState::FloatValue:
    case TokenizerState::Identifier:
        tokenValue += ch;
        break;
    case TokenizerState::Breaket:
        tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    case TokenizerState::Punctuation:
        tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    case TokenizerState::Operator:
        tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    }
}

bool tokenCanBeCreated(const TokenizerState & parserState)
{
    return parserState != TokenizerState::Space && parserState != TokenizerState::Start;
}

Token createToken(const TokenizerState & parserState, const std::string & tokenValue)
{
    switch (parserState)
    {
    case TokenizerState::IntValue:
        return Token(Token::TokenType::IntValue, tokenValue);
    case TokenizerState::FloatValue:
        return Token(Token::TokenType::FloatValue, tokenValue);
    case TokenizerState::Identifier:
        return Token(
            Token::isKeyword(tokenValue)
                ? Token::TokenType::Keyword
                : Token::TokenType::Identifier,
            tokenValue
        );
    case TokenizerState::Breaket:
        return Token(Token::TokenType::Breaket, tokenValue);
    case TokenizerState::Punctuation:
        return Token(Token::TokenType::Punctuation, tokenValue);
    case TokenizerState::Operator:
        return Token(Token::TokenType::Operator, tokenValue);
    }
    throw std::exception();
}

void processBreaket(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_front(createToken(parserState, tokenValue));
    }
    tokenValue = { ch };
    parserState = TokenizerState::Breaket;
}

void processPunctuation(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_front(createToken(parserState, tokenValue));
    }
    tokenValue = { ch };
    parserState = TokenizerState::Punctuation;
}

void processOperatorCh(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    if (parserState == TokenizerState::Operator)
    {
        if (tokenValue == "/")
        {
            tokenValue = "";
            parserState = TokenizerState::Comment;
        }
        else
        {
            tokenValue += ch;
        }
    }
    else
    {
        if (tokenCanBeCreated(parserState))
        {
            tokens.push_front(createToken(parserState, tokenValue));
        }
        tokenValue = { ch };
        parserState = TokenizerState::Operator;
    }
}

void processSpace(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_front(createToken(parserState, tokenValue));
    }
    tokenValue = "";
    parserState = TokenizerState::Space;
}

void processDot(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch) 
{
    switch (parserState)
    {
    case TokenizerState::IntValue:
        tokenValue += ch;
        parserState = TokenizerState::FloatValue;
        break;
    case TokenizerState::FloatValue:
    case TokenizerState::Identifier:
    case TokenizerState::Breaket:
    case TokenizerState::Punctuation:
    case TokenizerState::Operator:
    case TokenizerState::Space:
    case TokenizerState::Start:
        throw UnexpectedSymbolExeption(ch);
    }
}

class Tokenizer
{
public:
    static std::list<Token> parse(std::istream & input)
    {
        std::list<Token> tokens = std::list<Token>();
        std::string tokenValue = "";
        TokenizerState parserState = TokenizerState::Start;
        char ch;
        while (input.get(ch)) 
        {
            if (parserState == TokenizerState::Comment)
            {
                if (ch == '\n')
                {
                    parserState = TokenizerState::Space;
                }
                continue;
            }
            if (ch == '.')
            {
                processDot(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (isalpha(ch))
            {
                processAlphabitCh(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (isdigit(ch))
            {
                processDigit(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (ch == ')'
                || ch == '('
                || ch == '{'
                || ch == '}')
            {
                processBreaket(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (ch == ';'
                || ch == ',')
            {
                processPunctuation(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (ch == '+'
                || ch == '-'
                || ch == '/'
                || ch == '*'
                || ch == '|'
                || ch == '&')
            {
                processOperatorCh(tokens, parserState, tokenValue, ch);
                continue;
            }
            if (ch == '\n'
                || ch == ' '
                || ch == '\t')
            {
                processSpace(tokens, parserState, tokenValue, ch);
                continue;
            }
            throw UnexpectedSymbolExeption(ch);
        }
        return tokens;
    }
};
 