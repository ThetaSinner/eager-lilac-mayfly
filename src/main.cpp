#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/stream-dump.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

int main(int argc, char** args) {
  Logger* log = LoggerSharedInstance::get();

  #ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
  #else
  log->at(Level::Info) << "This is a release build.\n";
  #endif

  LoggerSharedInstance::get()->setLevel(levelToInt(Level::Info));

  EchelonCompiler compiler;

  try {
    auto ast = compiler.parse("");
    stream_dump(Level::Info, ast);
  }
  catch (const std::runtime_error& e) {
    LoggerSharedInstance::get()->at(Level::Fatal) << "dev compile failed [" << e.what() << "]";
    return 1;
  }

  log->at(Level::Info) << "\nProgram will exit normally.\n";
  return 0;
}
