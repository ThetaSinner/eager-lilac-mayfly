#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

#include <vector>
#include <string>

enum class TokenTypeEnum {
  String,
  Identifier,
  Integer,
  BlockDelimO,
  BlockDelimC,
  Paren,
  ParenO,
  ParenC,
  OperatorAdd,
  SqBracket,
  Assign,
  Equality,
  SingleLineComment,
  MultiLineComment,
  StructureOperator,
  AndOperator,

  #ifdef ECHELON_DEBUG
  First = String,
  Last = AndOperator,
  #endif
};

#endif
