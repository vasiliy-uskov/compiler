#include "Tokenizer.h"

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

UnexpectedSymbolExeption::UnexpectedSymbolExeption(char symbol, const TokenPosition & position)
    : _symbol(symbol), _position(position) {}
    
const char* UnexpectedSymbolExeption::what() const noexcept
{
    std::ostringstream message;
    message << "Unexpected symbol(" << _position.line << "," <<  _position.end - 1 << "): " << _symbol;
    return std::string(message.str()).c_str();
}

void processAlphabitCh(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
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
        tokens.push_back({ TokenType::Breaket, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::Punctuation:
        tokens.push_back({ TokenType::Punctuation, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::Operator:
        tokens.push_back({ TokenType::Operator, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::Identifier;
        break;
    case TokenizerState::IntValue:
    case TokenizerState::FloatValue:
        throw UnexpectedSymbolExeption(ch, position);
    }
}

void processDigit(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
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
        tokens.push_back({ TokenType::Breaket, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    case TokenizerState::Punctuation:
        tokens.push_back({ TokenType::Punctuation, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    case TokenizerState::Operator:
        tokens.push_back({ TokenType::Operator, tokenValue, position });
        tokenValue = { ch };
        parserState = TokenizerState::IntValue;
        break;
    }
}

bool tokenCanBeCreated(const TokenizerState & parserState)
{
    return parserState != TokenizerState::Space
    && parserState != TokenizerState::Start
    && parserState != TokenizerState::Comment;
}

Token createToken(const TokenizerState & parserState, const std::string & tokenValue, const TokenPosition & position)
{
    switch (parserState)
    {
    case TokenizerState::IntValue:
        return { TokenType::IntValue, tokenValue, position };
    case TokenizerState::FloatValue:
        return { TokenType::FloatValue, tokenValue, position };
    case TokenizerState::Identifier:
        return {
            Token::isKeyword(tokenValue)
                ? TokenType::Keyword
                : TokenType::Identifier,
            tokenValue,
            position
        };
    case TokenizerState::Breaket:
        return { TokenType::Breaket, tokenValue, position };
    case TokenizerState::Punctuation:
        return { TokenType::Punctuation, tokenValue, position };
    case TokenizerState::Operator:
        return { TokenType::Operator, tokenValue, position };
    }
    throw std::exception();
}

void processBreaket(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_back(createToken(parserState, tokenValue, position));
    }
    tokenValue = { ch };
    parserState = TokenizerState::Breaket;
}

void processPunctuation(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_back(createToken(parserState, tokenValue, position));
    }
    tokenValue = { ch };
    parserState = TokenizerState::Punctuation;
}

void processOperatorCh(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
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
            tokens.push_back(createToken(parserState, tokenValue, position));
        }
        tokenValue = { ch };
        parserState = TokenizerState::Operator;
    }
}

void processSpace(std::list<Token> & tokens, TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
{
    if (tokenCanBeCreated(parserState))
    {
        tokens.push_back(createToken(parserState, tokenValue, position));
    }
    tokenValue = "";
    parserState = TokenizerState::Space;
}

void processDot(TokenizerState & parserState, std::string & tokenValue, char ch, const TokenPosition & position) 
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
        throw UnexpectedSymbolExeption(ch, position);
    }
}

std::list<Token> Tokenizer::parse(std::istream & input)
{
    std::list<Token> tokens = std::list<Token>();
    std::string tokenValue = "";
    TokenizerState parserState = TokenizerState::Start;
    size_t line = 1;
    size_t chPosition = 1;
    char ch;
    while (input.get(ch)) 
    {
        chPosition++;
        if (ch != '\n' && parserState == TokenizerState::Comment)
        {
            continue;
        }
        if (ch == '\n')
        {
            processSpace(tokens, parserState, tokenValue, ch, { line, chPosition });
            line++;
            chPosition = 0;
        }
        else if (ch == '.')
        {
            processDot(parserState, tokenValue, ch, { line, chPosition });
        }
        else if (isalpha(ch))
        {
            processAlphabitCh(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else if (isdigit(ch))
        {
            processDigit(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else if (ch == ')'
            || ch == '('
            || ch == '{'
            || ch == '}')
        {
            processBreaket(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else if (ch == ';' || ch == ',')
        {
            processPunctuation(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else if (ch == '+'
            || ch == '-'
            || ch == '/'
            || ch == '*'
            || ch == '|'
            || ch == '&'
            || ch == '='
            || ch == '<'
            || ch == '>'
            || ch == '!')
        {
            processOperatorCh(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else if (ch == ' ' || ch == '\t')
        {
            processSpace(tokens, parserState, tokenValue, ch, { line, chPosition });
        }
        else
        {
            throw UnexpectedSymbolExeption(ch, { line, chPosition });            
        }
    }
    return tokens;
}
 