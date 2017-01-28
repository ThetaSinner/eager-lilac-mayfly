#ifndef AST_NODE_TYPE_ENUM_HPP_INCLUDED
#define AST_NODE_TYPE_ENUM_HPP_INCLUDED

enum class AstNodeType {
  Program,
  Block,
  Variable,
  VariableValue,
  TypeName,
  ExplicitType,
  Package,
  Enum,
  EnumConstant,
  EnumConstants,
  Module,
  BinaryOperatorAdd,
  BinaryOperatorSubtract,
  BinaryOperatorMultiply,
  BinaryOperatorDivide,
  LogicalDisjunction,
  LogicalConjunction,
  Function,
  FunctionParamDefinitions,
  FunctionParamDefinition,
  FunctionCall,
  FunctionCallParams,
  FunctionCallParam,
  Type,
  String,
  Integer,
  Decimal,
  BooleanTrue,
  BooleanFalse,
  EqualityOperator,
  If,
  ElseIf,
  Else,
  Branches,
  SingleLineComment,
  MultiLineComment,
  Until,
  While,
  Each,
  Condition,
  Behaviour,
  ExpressionRange,
  ExpressionRangeBegin,
  ExpressionRangeEnd,
  Iterator,
  ExprGroup,
  LogicalNegation,
  UnaryMinus,
  AccessSpecification,
  AccessSpecifierPublic,
  NameStructure,
  Expression,

  // These are not for program structure.
  First = Program,
  Last = Expression,
};

#endif
