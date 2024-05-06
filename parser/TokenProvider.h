#pragma once
#include <list>
#include <optional>
#include "../tokenizer/Token.h"

typedef std::list<Token>::const_iterator TokenIterator;
class TokenProvider {
private:
    TokenIterator current;
    TokenIterator end;
public:
    TokenProvider(const TokenIterator & current, const TokenIterator & end);
    TokenProvider next() const;
    Token get() const;
    TokenIterator getIt() const;
    bool isEnd() const;
    TokenProvider & operator=(const TokenProvider & other);
};