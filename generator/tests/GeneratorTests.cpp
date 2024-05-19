#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include "variables.h"
#include "../../tokenizer/Tokenizer.h"
#include "../../parser/Parser.h"
#include "../Generator.h"

SyntaxTree getSyntaxTree(const std::string & fileName)
{
    std::fstream input;
    std::fstream realOutput;

    input.open(TESTS_DIR + fileName, std::ios::in);

    try
    {
        auto tokens = Tokenizer::parse(input);
        return Parser::parse(tokens);
    }
    catch(const UnexpectedTokenException& e)
    {
        std::cerr << e.realToken.value << '\n';
        std::cerr << e.realToken.position << '\n';
        std::cerr << e.expectedToken << '\n';
        throw e;
    }
}

TEST(ProgrammObjectModel, CheckTypesInCorrectProgramWithoutErrors)
{
    auto tree = getSyntaxTree("correctProgramm.txt");
    auto programm = Programm(tree);
    auto errors = programm.initIdentifiersScope();
    errors.add(programm.checkTypes());
    EXPECT_TRUE(errors.isEmpty());
}
TEST(ProgrammObjectModel, CheckInvalidProgramTypesWithErrors)
{
    auto programm = Programm(getSyntaxTree("invalidProgramm.txt"));
    auto errors = programm.initIdentifiersScope();
    errors.add(programm.checkTypes());
    EXPECT_TRUE(!errors.isEmpty());
}