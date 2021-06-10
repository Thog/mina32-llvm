//===-- MINA32Disassembler.cpp - Disassembler for MINA32 ------------------===//
//
// Part of the LLVM fork for the MINA32 project, under the Apache License v2.0
// with LLVM Exceptions. See https://llvm.org/LICENSE.txt for license
// information.
// SPDX-License-Identifier: Apache-2.0 with LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the MINA32Disassembler class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MINA32MCTargetDesc.h"
#include "TargetInfo/MINA32TargetInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "mina32-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {
class MINA32Disassembler : public MCDisassembler {

public:
  MINA32Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end anonymous namespace

static MCDisassembler *createMINA32Disassembler(const Target &T,
                                               const MCSubtargetInfo &STI,
                                               MCContext &Ctx) {
  return new MINA32Disassembler(STI, Ctx);
}

extern "C" void LLVMInitializeMINA32Disassembler() {
  // Register the disassembler for each target.
  TargetRegistry::RegisterMCDisassembler(getTheMINA32Target(),
                                         createMINA32Disassembler);
}

static const unsigned GPRDecoderTable[] = {
  MINA32::R0, MINA32::R1, MINA32::R2, MINA32::R3,
  MINA32::R4, MINA32::R5, MINA32::R6, MINA32::R7,
  MINA32::R8, MINA32::R9, MINA32::R10, MINA32::R11,
  MINA32::R12, MINA32::R13, MINA32::R14, MINA32::SP
};

static DecodeStatus DecodeGPRRegisterClass(MCInst &Inst, uint64_t RegNo,
                                           uint64_t Address,
                                           const void *Decoder) {
  if (RegNo > sizeof(GPRDecoderTable))
    return MCDisassembler::Fail;

  unsigned Reg = GPRDecoderTable[RegNo];
  Inst.addOperand(MCOperand::createReg(Reg));
  return MCDisassembler::Success;
}

template <unsigned N>
static DecodeStatus decodeImmOperand(MCInst &Inst, uint64_t Imm,
                                     int64_t Address, const void *Decoder) {
  int Shift = Imm >> 12;
  int Base = SignExtend32<12>(Imm & 0xfff);

  Inst.addOperand(MCOperand::createImm(Base << (Shift + N)));
  return MCDisassembler::Success;
}

static DecodeStatus decodeAddrOperand(MCInst &Inst, uint64_t Imm,
                                      int64_t Address, const void *Decoder) {
  int Base = SignExtend32<24>(Imm & 0xffffff);
  Inst.addOperand(MCOperand::createImm(Base << 2));
  return MCDisassembler::Success;
}

#include "MINA32GenDisassemblerTables.inc"

namespace llvm {
extern const MCInstrDesc MINA32Insts[];
} // end namespace llvm

DecodeStatus MINA32Disassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                               ArrayRef<uint8_t> Bytes,
                                               uint64_t Address,
                                               raw_ostream &CS) const {
  Size = 4;
  if (Bytes.size() < 4) {
    Size = 0;
    return MCDisassembler::Fail;
  }

  // Get the four bytes of the instruction.
  uint32_t Inst = support::endian::read32le(Bytes.data());
  return decodeInstruction(DecoderTableMINA3232, MI, Inst, Address, this, STI);
}
