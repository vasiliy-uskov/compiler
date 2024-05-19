#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include "variables.h"
#include "../../tokenizer/Tokenizer.h"
#include "../../parser/Parser.h"
#include "../Generator.h"

void checkLine(const SyntaxTree & node, size_t line)
{
    EXPECT_EQ(node.tokens[0].position.line, line);
}

SyntaxTree getSyntaxTree(const std::string & fileName)
{
    std::fstream input;
    std::fstream realOutput;

    input.open(TESTS_DIR + fileName, std::ios::in);

    auto tokens = Tokenizer::parse(input);
    return Parser::parse(tokens);
}

TEST(ProgrammObjectModel, CheckTypesInCorrectProgramWithoutErrors)
{
    auto programm = Programm(getSyntaxTree("correctProgramm.txt"));
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
    
    EXPECT_EQ(errors.unknownFunctionsErrors.size(), 1);
    checkLine(errors.unknownFunctionsErrors[0].node, 90);

    EXPECT_EQ(errors.dublicateDeclarations.size(), 2);
    checkLine(errors.dublicateDeclarations[0].node, 10);
    checkLine(errors.dublicateDeclarations[1].node, 20);

    EXPECT_EQ(errors.argumentsCountErrors.size(), 4);
    checkLine(errors.argumentsCountErrors[0].node, 32);
    checkLine(errors.argumentsCountErrors[1].node, 33);
    checkLine(errors.argumentsCountErrors[2].node, 36);
    checkLine(errors.argumentsCountErrors[3].node, 37);
        
    EXPECT_EQ(errors.unknownVariablesErrors.size(), 6);
    checkLine(errors.unknownVariablesErrors[0].node, 22);
    checkLine(errors.unknownVariablesErrors[1].node, 24);
    checkLine(errors.unknownVariablesErrors[2].node, 65);
    checkLine(errors.unknownVariablesErrors[3].node, 66);
    checkLine(errors.unknownVariablesErrors[4].node, 67);
    checkLine(errors.unknownVariablesErrors[5].node, 68);

    EXPECT_EQ(errors.invalidTypeErrors.size(), 41);
    checkLine(errors.invalidTypeErrors[0].node, 3);
    checkLine(errors.invalidTypeErrors[1].node, 21);
    checkLine(errors.invalidTypeErrors[2].node, 25);
    checkLine(errors.invalidTypeErrors[3].node, 26);
    checkLine(errors.invalidTypeErrors[4].node, 27);
    checkLine(errors.invalidTypeErrors[5].node, 28);
    checkLine(errors.invalidTypeErrors[6].node, 29);
    checkLine(errors.invalidTypeErrors[7].node, 31);
    checkLine(errors.invalidTypeErrors[8].node, 31);
    checkLine(errors.invalidTypeErrors[9].node, 35);
    checkLine(errors.invalidTypeErrors[10].node, 35);
    checkLine(errors.invalidTypeErrors[11].node, 38);
    checkLine(errors.invalidTypeErrors[12].node, 40);
    checkLine(errors.invalidTypeErrors[13].node, 41);
    checkLine(errors.invalidTypeErrors[14].node, 43);
    checkLine(errors.invalidTypeErrors[15].node, 43);
    checkLine(errors.invalidTypeErrors[16].node, 44);
    checkLine(errors.invalidTypeErrors[17].node, 44);
    checkLine(errors.invalidTypeErrors[18].node, 45);
    checkLine(errors.invalidTypeErrors[19].node, 45);
    checkLine(errors.invalidTypeErrors[20].node, 46);
    checkLine(errors.invalidTypeErrors[21].node, 46);
    checkLine(errors.invalidTypeErrors[22].node, 47);
    
    checkLine(errors.invalidTypeErrors[23].node, 49);
    checkLine(errors.invalidTypeErrors[24].node, 49);
    checkLine(errors.invalidTypeErrors[25].node, 50);
    checkLine(errors.invalidTypeErrors[26].node, 50);
    checkLine(errors.invalidTypeErrors[27].node, 51);
    checkLine(errors.invalidTypeErrors[28].node, 51);
    checkLine(errors.invalidTypeErrors[29].node, 52);
    checkLine(errors.invalidTypeErrors[30].node, 52);
    checkLine(errors.invalidTypeErrors[31].node, 53);
    checkLine(errors.invalidTypeErrors[32].node, 53);
    checkLine(errors.invalidTypeErrors[33].node, 54);
    checkLine(errors.invalidTypeErrors[34].node, 54);
    checkLine(errors.invalidTypeErrors[35].node, 70);
    checkLine(errors.invalidTypeErrors[36].node, 73);
    checkLine(errors.invalidTypeErrors[37].node, 78);
    checkLine(errors.invalidTypeErrors[38].node, 81);
    checkLine(errors.invalidTypeErrors[39].node, 84);
    checkLine(errors.invalidTypeErrors[40].node, 87);
}

