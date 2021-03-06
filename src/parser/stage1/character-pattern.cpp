#include <echelon/parser/stage1/character-pattern.hpp>

void CharacterPattern::addGroup(CharacterPatternGroup *cpg) {
  groups.push_back(cpg);
}

std::list<CharacterPatternGroup *> *CharacterPattern::getGroups() {
  return &groups;
}

void CharacterPattern::setTokenType(TokenType type) {
  this->type = type;
}

TokenType CharacterPattern::getTokenType() {
  return type;
}