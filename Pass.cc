#include <vector>

#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

struct CustomPass : public ModulePass {
  CustomPass() : ModulePass(ID) {}

  virtual bool runOnModule(Module &M) override;
  void instrumentMain(Module &M, Function &MainFn);

  static inline char ID = 0;
};

}  // namespace

bool CustomPass::runOnModule(Module &M) {
  errs() << "runOnModule\n";

  for (auto &F : M) {
    if (F.getName() == "main") {
      errs() << F.getName() << '\n';
      instrumentMain(M, F);
    }
  }

  return false;
}

void CustomPass::instrumentMain(Module &M, Function &MainFn) {
  LLVMContext& context = MainFn.getContext();
  const auto VoidTy = Type::getVoidTy(context);
  const auto Int8Ty = Type::getInt8Ty(context);
  const auto Int32Ty = Type::getInt32Ty(context);

  // 1. Insert a call to debug() in main().
  FunctionType *FnTy = FunctionType::get(VoidTy, {Int32Ty}, false);
  FunctionCallee DebugFn = M.getOrInsertFunction("debug", FnTy);

  auto &BB = MainFn.front();
  BasicBlock::iterator IP = BB.getFirstInsertionPt();

  IRBuilder<> IRB(&(*IP));
  IRB.CreateCall(DebugFn, ConstantInt::get(Int32Ty, 9527));

  // 2. Make argv[1] = "aesophor is ghost !!!"
  //    MainFn.getArg(1) -> i8** %1
  Value *NewArgv1 = IRB.CreateGlobalStringPtr("aesophor is ghost !!!");
  Value *Argv1 = IRB.CreateGEP(
      PointerType::get(Int8Ty, 0), MainFn.getArg(1), ConstantInt::get(Int32Ty, 1));
  IRB.CreateStore(NewArgv1, Argv1);

  // 3. Make argc = 9487
  MainFn.getArg(0)->replaceAllUsesWith(ConstantInt::get(Int32Ty, 9487));
}

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new CustomPass());
}

static RegisterStandardPasses RegisterPass(
    PassManagerBuilder::EP_OptimizerLast, registerPass);

static RegisterStandardPasses RegisterPass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerPass);
