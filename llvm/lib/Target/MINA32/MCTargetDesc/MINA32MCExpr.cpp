//===-- MINA32MCExpr.cpp - MINA32-specific MC expression classes ----------===//
//
// Part of the LLVM fork for the MINA32 project, under the Apache License v2.0
// with LLVM Exceptions. See https://llvm.org/LICENSE.txt for license
// information.
// SPDX-License-Identifier: Apache-2.0 with LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MINA32MCExpr.h"

#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCValue.h"

#define DEBUG_TYPE "MINA32mcexpr"

namespace llvm {

const MINA32MCExpr *MINA32MCExpr::create(VariantKind Kind, const MCExpr *Expr,
                                         MCContext &Ctx) {
  return new (Ctx) MINA32MCExpr(Kind, Expr);
}

bool MINA32MCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                             MCAsmLayout const *Layout,
                                             MCFixup const *Fixup) const {
  return Expr->evaluateAsRelocatable(Res, Layout, Fixup);
}

void MINA32MCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  if (Kind == VK_MINA32_LO) OS << "%lo(";
  if (Kind == VK_MINA32_HI) OS << "%hi(";

  Expr->print(OS, MAI);

  if (Kind != VK_MINA32_None)
    OS << ')';
}

void MINA32MCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*Expr);
}

MINA32MCExpr::VariantKind MINA32MCExpr::getVariantKindForName(StringRef name) {
  return StringSwitch<MINA32MCExpr::VariantKind>(name)
      .Case("lo", VK_MINA32_LO)
      .Case("hi", VK_MINA32_HI)
      .Default(VK_MINA32_None);
}

MCFragment *MINA32MCExpr::findAssociatedFragment() const {
  return Expr->findAssociatedFragment();
}

void MINA32MCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
  // TODO?
}

} // end namespace llvm
