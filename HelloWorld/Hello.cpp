#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
// This method implements what the pass does
void visitor(Function &F) {
    errs() << "(llvm-tutor)   Function name is : "<< F.getName() << "\n";
    errs() << "(llvm-tutor)   number of arguments : " << F.arg_size() << "\n";
    errs() << "(llvm-tutor)   Function type is : " << F.getFunctionType() << "\n";
    errs() << "(llvm-tutor)   Function dump is : \n";
    F.dump();
    errs() << "\n";
    errs() << "From Harrisons!\n";
}
struct Hello : PassInfoMixin<Hello> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        visitor(F);
        return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
};
} // namespace

llvm::PassPluginLibraryInfo getHelloWorldPluginInfoHello() {
  return {LLVM_PLUGIN_API_VERSION, "Hello", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hello") {
                    FPM.addPass(Hello());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHelloWorldPluginInfoHello();
}