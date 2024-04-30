#include <string>
#include <array>
#include <algorithm>

class UnknownOperatorExeption: public std::exception
{
private:
    const std::string _operator;
public:
    UnknownOperatorExeption(const std::string & op) : _operator(op) {}
    
    const char* what() const noexcept override
    {
        const std::string message = "Unknown operator: " + _operator;
        return message.c_str();
    }
};

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

struct Token
{
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

    const TokenType type;
    const std::string value;

    Token(const TokenType & type, const std::string & value)
        : type(type), value(value)
    {
        if (type == TokenType::Operator && !isOperatorExist(value))
        {
            throw UnknownOperatorExeption(value);
        }
    }
    
    static bool isKeyword(const std::string & keyword)
    {
        return inArr(definedKeywords, keyword);
    }
};



std::ostream& operator<<(std::ostream& os, const Token::TokenType type)
{
    switch(type)
    {
        case Token::TokenType::Identifier:
            return os << std::string("Identifier");
        case Token::TokenType::Keyword:
            return os << std::string("Keyword");
        case Token::TokenType::IntValue:
            return os << std::string("IntValue");
        case Token::TokenType::FloatValue:
            return os << std::string("FloatValue");
        case Token::TokenType::Punctuation:
            return os << std::string("Punctuation");
        case Token::TokenType::Breaket:
            return os << std::string("Breaket");
        case Token::TokenType::Operator:
            return os << std::string("Operator");
    }

    return os;
}
