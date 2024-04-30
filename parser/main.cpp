#include <iostream>
#include "Parser.cpp"

int main()
{
    try {
        const std::list<Token> tokens = Parser::parse(std::cin);
        std::for_each(tokens.begin(), tokens.end(), [](Token token) {
            std::cout << token.type << ' ' << token.value << std::endl;
        });
    }
    catch (UnexpectedSymbolExeption ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (UnknownOperatorExeption ex) {
        std::cout << ex.what() << std::endl;
    }
}