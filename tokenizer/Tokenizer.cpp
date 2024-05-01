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

Token createIdentificatorStateToken(const std::string & tokenValue) {
    return Token::isKeyword(tokenValue)
        ? Token(Token::TokenType::Keyword, tokenValue)
        : Token(Token::TokenType::Identifier, tokenValue);
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
            if (isalpha(ch))
            {
                switch (parserState)
                {
                case TokenizerState::Start:
                case TokenizerState::Space:
                case TokenizerState::Identifier:
                    tokenValue += ch;
                    parserState = TokenizerState::Identifier;
                    continue;
                case TokenizerState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    tokenValue = { ch };
                    parserState = TokenizerState::Identifier;
                    continue;
                case TokenizerState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    tokenValue = { ch };
                    parserState = TokenizerState::Identifier;
                    continue;
                case TokenizerState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    tokenValue = { ch };
                    parserState = TokenizerState::Identifier;
                    continue;
                }
            }
            if (isdigit(ch))
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
                continue;
            }
            if (ch == ')'
                || ch == '('
                || ch == '{'
                || ch == '}')
            {
                switch (parserState)
                {
                case TokenizerState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case TokenizerState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case TokenizerState::Identifier:
                    tokens.push_front(createIdentificatorStateToken(tokenValue));
                    break;
                case TokenizerState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case TokenizerState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case TokenizerState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = { ch };
                parserState = TokenizerState::Breaket;
                continue;
            }
            if (ch == ';'
                || ch == ',')
            {
                switch (parserState)
                {
                case TokenizerState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case TokenizerState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case TokenizerState::Identifier:
                    tokens.push_front(createIdentificatorStateToken(tokenValue));
                    break;
                case TokenizerState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case TokenizerState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case TokenizerState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = { ch };
                parserState = TokenizerState::Punctuation;
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
                case TokenizerState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case TokenizerState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case TokenizerState::Identifier:
                    tokens.push_front(createIdentificatorStateToken(tokenValue));
                    break;
                case TokenizerState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case TokenizerState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case TokenizerState::Operator:
                    if (tokenValue == "/")
                    {
                        tokenValue = "";
                        parserState = TokenizerState::Comment;
                        continue;
                    }
                    break;
                }
                tokenValue += ch;
                parserState = TokenizerState::Operator;
                continue;
            }
            if (ch == '\n'
                || ch == ' '
                || ch == '\t')
            {
                switch (parserState)
                {
                case TokenizerState::IntValue:
                    tokens.push_front(Token(Token::TokenType::IntValue, tokenValue));
                    break;
                case TokenizerState::FloatValue:
                    tokens.push_front(Token(Token::TokenType::FloatValue, tokenValue));
                    break;
                case TokenizerState::Identifier:
                    tokens.push_front(createIdentificatorStateToken(tokenValue));
                    break;
                case TokenizerState::Breaket:
                    tokens.push_front(Token(Token::TokenType::Breaket, tokenValue));
                    break;
                case TokenizerState::Punctuation:
                    tokens.push_front(Token(Token::TokenType::Punctuation, tokenValue));
                    break;
                case TokenizerState::Operator:
                    tokens.push_front(Token(Token::TokenType::Operator, tokenValue));
                    break;
                }
                tokenValue = "";
                parserState = TokenizerState::Space;
                continue;
            }
            if (ch == '.' && parserState == TokenizerState::IntValue)
            {
                tokenValue += ch;
                parserState = TokenizerState::FloatValue;
                continue;
            }
            throw UnexpectedSymbolExeption(ch);
        }
        return tokens;
    }
};
 