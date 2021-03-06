#include <echelon/model/ast/ast-node.hpp>
#include <stdexcept>

#ifdef ECHELON_DEBUG

#include <iostream>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>

#endif

void AstNode::putChild(AstNode *child) {
  children.push_back(child);
}

void AstNode::putChildFront(AstNode *child) {
  children.insert(children.begin(), child);
}

int AstNode::getChildCount() {
  return children.size();
}

AstNode *AstNode::getChild(unsigned index) {
  return children.at(index);
}

bool AstNode::hasChild(AstNodeType childType) {
  for (auto i : children) {
    if (i->getType() == childType) {
      return true;
    }
  }

  return false;
}

AstNode *AstNode::getChild(AstNodeType childType) {
  for (auto i : children) {
    if (i->getType() == childType) {
      return i;
    }
  }

#ifdef ECHELON_DEBUG
  LoggerSharedInstance::get()->at(Level::Debug) << "Request for child by childType ["
                                                << EchelonLookup::getInstance()->toString(childType) << "] failed.\n";
  throw std::runtime_error("Request for child by type failed.");
#else
  return nullptr;
#endif
}

AstNode* AstNode::getLastChild() {
  return getChild(getChildCount() - 1);
}

void AstNode::setType(AstNodeType type) {
  this->type = type;
}

AstNodeType AstNode::getType() {
  return type;
}

void AstNode::setData(std::string data) {
  this->data = data;
}

std::string AstNode::getData() {
  return data;
}
