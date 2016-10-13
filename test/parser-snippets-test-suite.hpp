#include <cxxtest/TestSuite.h>
#include "elm/parser/tokenizer.hpp"

#include <fstream>
#include <sstream>
#include <string>

#ifndef SNIPPETS_DIR
  #define SNIPPETS_DIR "../../snippets"
#endif

class ParserSnippetTestSuite : public CxxTest::TestSuite
{
private:
  bool fileExists(std::string& fname) {
    std::ifstream f(fname.c_str());
    return f.good();
  }

  std::string getFileContents(std::string& fname) {
    if (!fileExists(fname)) {
        std::cout << "Missing file " << fname << std::endl;
        TS_ASSERT(false);
    }

    std::ifstream in(fname.c_str());
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
  }

public:
    void test_Fundamentals_Types(void) {
      std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/types.elm";

      Tokenizer t;
      auto res = t.tokenize(getFileContents(file));

      TS_ASSERT_EQUALS(4, res.size());
      TS_ASSERT_EQUALS("type", res.at(0));
      TS_ASSERT_EQUALS("MyType", res.at(1));
      TS_ASSERT_EQUALS("{", res.at(2));
      TS_ASSERT_EQUALS("}", res.at(3));
    }

    void test_Fundamentals_Comment(void) {
      std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/comment.elm";

      Tokenizer t;
      auto res = t.tokenize(getFileContents(file));

      TS_ASSERT_EQUALS(3, res.size());
      TS_ASSERT_EQUALS("// Single line comment", res.at(0));
      TS_ASSERT_EQUALS("/**/", res.at(1));
      TS_ASSERT_EQUALS("/*\n * Multi-line comment\n */", res.at(2));
    }
	
	void test_Fundamentals_Enum(void) {
		std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/enum.elm";

      Tokenizer t;
      auto res = t.tokenize(getFileContents(file));

      TS_ASSERT_EQUALS(6, res.size());
      TS_ASSERT_EQUALS("enum", res.at(0));
      TS_ASSERT_EQUALS("MyEnum", res.at(1));
      TS_ASSERT_EQUALS("{", res.at(2));
	  TS_ASSERT_EQUALS("EnumConstOne", res.at(3));
	  TS_ASSERT_EQUALS("EnumConstTwo", res.at(4));
	  TS_ASSERT_EQUALS("}", res.at(5));
	}
};
