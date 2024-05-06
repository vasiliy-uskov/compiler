#include "TokenProvider.h"

TokenProvider::TokenProvider(const TokenIterator & current, const TokenIterator & end) : current(current), end(end) {}

TokenProvider TokenProvider::next() const
{
    return TokenProvider(std::next(current, 1), end);
}

Token TokenProvider::get() const
{
    return *current;
}

TokenIterator TokenProvider::getIt() const
{
    return current;
}

bool TokenProvider::isEnd() const
{
    return current == end;
}
TokenProvider & TokenProvider::operator=(const TokenProvider & other)
{
    current = other.current;
    end = other.end;
    return *this;
}