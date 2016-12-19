#include <cxxtest/TestSuite.h>

#include <list>

#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/parser/stage1/token.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>

class ParserStage2TestSuite : public CxxTest::TestSuite
{
private:
  Parser2 parser;

public:
  ParserStage2TestSuite() {
    loadParserStage2Data();
  }

  void test_packageDeclaration(void) {
    std::list<Token*> program;
    program.push_back(new Token("package", TokenType::Identifier));
    program.push_back(new Token("echelon", TokenType::Identifier));
    program.push_back(new Token("::", TokenType::StructureOperator));
    program.push_back(new Token("test_package", TokenType::Identifier));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());
    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("echelon", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());
    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("test_package", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_LongPackageDeclaration(void) {
    std::list<Token*> program;
    program.push_back(new Token("package", TokenType::Identifier));
    program.push_back(new Token("test", TokenType::Identifier));
    program.push_back(new Token("::", TokenType::StructureOperator));
    program.push_back(new Token("pack", TokenType::Identifier));
    program.push_back(new Token("::", TokenType::StructureOperator));
    program.push_back(new Token("name", TokenType::Identifier));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("test", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("pack", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_nestedModule(void) {
    std::list<Token*> program;
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("name", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("}", TokenType::BlockDelimC));
    program.push_back(new Token("}", TokenType::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("nested", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_SiblingModules() {
    std::list<Token*> program;

    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("name", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested1", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("}", TokenType::BlockDelimC));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested2", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("}", TokenType::BlockDelimC));
    program.push_back(new Token("}", TokenType::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("nested1", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS("nested2", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChildCount());
  }

  void testVariableDeclaration(void) {
    std::list<Token*> program;
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assign));
    program.push_back(new Token("helloWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token(")", TokenType::ParenC));
    program.push_back(new Token("+", TokenType::OperatorAdd));
    program.push_back(new Token("byeWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token(")", TokenType::ParenC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("helloWorld", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("byeWorld", ast -> getChild(0) -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChildCount());
  }

  void testVariableDeclarationWithType(void) {
    std::list<Token*> program;
    program.push_back(new Token("integer", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assign));
    program.push_back(new Token("helloWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token(")", TokenType::ParenC));
    program.push_back(new Token("+", TokenType::OperatorAdd));
    program.push_back(new Token("byeWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token(")", TokenType::ParenC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("integer", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("helloWorld", ast -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("byeWorld", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());
  }

  void testAddTwoStringsAndAssignToVariable(void) {
    std::list<Token*> program;
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assign));
    program.push_back(new Token("my string data ", TokenType::String));
    program.push_back(new Token("+", TokenType::OperatorAdd));
    program.push_back(new Token("other string data", TokenType::String));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("string", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("my string data ", ast -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("other string data", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());
  }

  void testAddThreeStringsAndAssignToVariable(void) {
    std::list<Token*> program;
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assign));
    program.push_back(new Token("my string data ", TokenType::String));
    program.push_back(new Token("+", TokenType::OperatorAdd));
    program.push_back(new Token("other string data", TokenType::String));
    program.push_back(new Token("+", TokenType::OperatorAdd));
    program.push_back(new Token("third string data", TokenType::String));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("string", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("my string data ", ast -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("other string data", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("third string data", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChild(1) -> getChildCount());
  }

  void testIfStatementWithEmptyBlock(void) {
    std::list<Token*> program;
    program.push_back(new Token("if", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token("1", TokenType::Integer));
    program.push_back(new Token("==", TokenType::Equality));
    program.push_back(new Token("2", TokenType::Integer));
    program.push_back(new Token(")", TokenType::ParenC));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("}", TokenType::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    auto branches = ast -> getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::Branches, branches -> getType());
    TS_ASSERT_EQUALS(1, branches -> getChildCount());

    auto _if = ast -> getChild(0) -> getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::If, _if -> getType());
    TS_ASSERT_EQUALS(1, _if -> getChildCount());

    auto eq_op = ast -> getChild(0) -> getChild(0) -> getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::EqualityOperator, eq_op -> getType());
    TS_ASSERT_EQUALS(2, eq_op -> getChildCount());

    auto int_1 = ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(0);
    TS_ASSERT_EQUALS("1", int_1 -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, int_1 -> getType());
    TS_ASSERT_EQUALS(0, int_1 -> getChildCount());

    auto int_2 = ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(1);
    TS_ASSERT_EQUALS("2", int_2 -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, int_2 -> getType());
    TS_ASSERT_EQUALS(0, int_2 -> getChildCount());
  }

  void testIfStatement(void) {
    std::list<Token*> program;
    program.push_back(new Token("if", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenO));
    program.push_back(new Token("1", TokenType::Integer));
    program.push_back(new Token("==", TokenType::Equality));
    program.push_back(new Token("2", TokenType::Integer));
    program.push_back(new Token(")", TokenType::ParenC));
    program.push_back(new Token("{", TokenType::BlockDelimO));
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("cond_var", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assign));
    program.push_back(new Token("this string will only be assigned if 1 and 2 are equal", TokenType::String));
    program.push_back(new Token("}", TokenType::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Branches, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::If, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::EqualityOperator, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("1", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("2", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("cond_var", ast -> getChild(0) -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("string", ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("this string will only be assigned if 1 and 2 are equal", ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());
  }
};
