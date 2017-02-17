#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/model/internal/enhanced-ast-block-node.hpp>
#include <echelon/model/internal/enhanced-ast-function-node.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

class ContextPathTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  ContextPathTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));
  }

  void testContextPaths() {
    std::stringstream ss;
    ss << "package MyProject::MyPackage ";
    ss << "module MyModule {";
    ss << "integer testVar = 5";
    ss << "function ibble() -> integer {5}";
    ss << "function ibble(integer t) -> integer {t + 5}";
    ss << "function dibble() {2}";
    ss << "module MySubModule {";
    ss << "function ibble() {6}";
    ss << "function quibble() {4}";
    ss << "type MyType {";
    ss << "function typeFunction() -> integer {}";
    ss << "}"; // my type.
    ss << "}"; // my sub module.
    ss << "function MySubModule::MyType::typeFunction() -> integer {integer someNumber = 1 someNumber + 15}";
    ss << "}"; // my module.

    auto out = compiler.enhance(ss.str());

    auto rootScope = ((EnhancedAstBlockNode*) out)->getScope();
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage", rootScope->getContext()->toString());
    auto myProjectPackage = out->getChild(EnhancedAstNodeType::Package);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject", AstEnhancerHelper::getContextAwareNode(myProjectPackage)->getContext()->toString());
    auto myPackagePackage = myProjectPackage->getChild(EnhancedAstNodeType::Package);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage", AstEnhancerHelper::getContextAwareNode(myPackagePackage)->getContext()->toString());

    auto myModule = myPackagePackage->getChild(EnhancedAstNodeType::Module);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule", AstEnhancerHelper::getContextAwareNode(myModule)->getContext()->toString());
    auto myModuleBlock = myModule->getChild(EnhancedAstNodeType::Block);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule", ((EnhancedAstBlockNode*) myModuleBlock)->getScope()->getContext()->toString());

    auto testVar = myModuleBlock->getChild(EnhancedAstNodeType::Variable);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/testVar", AstEnhancerHelper::getContextAwareNode(testVar)->getContext()->toString());

    auto ibble =  myModuleBlock->getChild(1);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/ibble-1", AstEnhancerHelper::getContextAwareNode(ibble)->getContext()->toString());
    auto ibbleint = myModuleBlock->getChild(2);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/ibble-2", AstEnhancerHelper::getContextAwareNode(ibbleint)->getContext()->toString());
    auto dibble = myModuleBlock->getChild(3);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/dibble-1", AstEnhancerHelper::getContextAwareNode(dibble)->getContext()->toString());

    auto mySubModule = myModuleBlock->getChild(EnhancedAstNodeType::Module);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/MySubModule", AstEnhancerHelper::getContextAwareNode(mySubModule)->getContext()->toString());
    auto mySubModuleBlock = mySubModule->getChild(EnhancedAstNodeType::Block);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/MySubModule", ((EnhancedAstBlockNode*) mySubModuleBlock)->getScope()->getContext()->toString());

    auto subIbble = mySubModuleBlock->getChild(0);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/MySubModule/ibble-1", AstEnhancerHelper::getContextAwareNode(subIbble)->getContext()->toString());
    auto quibble = mySubModuleBlock->getChild(1);
    TS_ASSERT_EQUALS("cp:/context-root/MyProject/MyPackage/MyModule/MySubModule/quibble-1", AstEnhancerHelper::getContextAwareNode(quibble)->getContext()->toString());

    /*
    log->at(Level::Info) << ((EnhancedAstCustomTypeNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::CustomType))->getContext()->toString() << "\n";
    log->at(Level::Info) << ((EnhancedAstBlockNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::CustomType)->getChild(EnhancedAstNodeType::Block))->getScope()->getContext()->toString() << "\n";

    log->at(Level::Info) << ((EnhancedAstFunctionNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::CustomType)->getChild(EnhancedAstNodeType::Block)->getChild(0))->getContext()->toString() << "\n";

    log->at(Level::Info) << ((EnhancedAstFunctionNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(5))->getContext()->toString() << "\n";
    log->at(Level::Info) << ((EnhancedAstBlockNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(5)->getChild(EnhancedAstNodeType::Block))->getScope()->getContext()->toString() << "\n";
    log->at(Level::Info) << ((EnhancedAstVariableNode*) myPackagePackage->getChild(EnhancedAstNodeType::Module)->getChild(EnhancedAstNodeType::Block)->getChild(5)->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::Variable))->getContext()->toString() << "\n";*/
  }
};