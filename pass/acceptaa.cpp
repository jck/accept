#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Pass.h"
#include "llvm/DataLayout.h"
#include "llvm/Support/raw_ostream.h"
#include "accept.h"

using namespace llvm;

namespace {
  struct AcceptAA : public ImmutablePass, public AliasAnalysis {
    static char ID;
    AcceptAA() : ImmutablePass(ID) {
      initializeAcceptAAPass(*PassRegistry::getPassRegistry());
    }

    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AliasAnalysis::getAnalysisUsage(AU);
      // dependencies?
    }

    virtual void initializePass() {
      InitializeAliasAnalysis(this);
    }

    virtual AliasResult alias(const Location &LocA, const Location &LocB) {
      errs() << "alias query\n";
      return MayAlias;
    }

    // This required bit works around C++'s multiple inheritance weirdness.
    // Casting this to AliasAnalysis* gets the correct vtable for those calls.
    virtual void *getAdjustedAnalysisPointer(const void *ID) {
      if (ID == &AliasAnalysis::ID)
        return (AliasAnalysis*)this;
      return this;
    }
  };
}

char AcceptAA::ID = 0;
INITIALIZE_AG_PASS(AcceptAA, AliasAnalysis, "accept-aa",
                   "ACCEPT approximate alias analysis",
                   false, true, false)

ImmutablePass *llvm::createAcceptAAPass() { return new AcceptAA(); }