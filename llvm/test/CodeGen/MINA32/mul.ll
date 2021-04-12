; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=mina32 -verify-machineinstrs < %s \
; RUN:   | FileCheck %s -check-prefix=M32I

define i32 @square(i32 %a) nounwind {
; M32I-LABEL: square:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -4
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 4
; M32I-NEXT:    mult r0, r0, r0
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    addi sp, sp, 4
; M32I-NEXT:    ret
  %1 = mul i32 %a, %a
  ret i32 %1
}

define i32 @mul(i32 %a, i32 %b) nounwind {
; M32I-LABEL: mul:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -4
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 4
; M32I-NEXT:    mult r0, r0, r1
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    addi sp, sp, 4
; M32I-NEXT:    ret
  %1 = mul i32 %a, %b
  ret i32 %1
}

define i32 @mul_constant(i32 %a) nounwind {
; M32I-LABEL: mul_constant:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -4
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 4
; M32I-NEXT:    multi r0, r0, 5
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    addi sp, sp, 4
; M32I-NEXT:    ret
  %1 = mul i32 %a, 5
  ret i32 %1
}

define i32 @mul_pow2(i32 %a) nounwind {
; M32I-LABEL: mul_pow2:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -4
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 4
; M32I-NEXT:    lsl r0, r0, 3
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    addi sp, sp, 4
; M32I-NEXT:    ret
  %1 = mul i32 %a, 8
  ret i32 %1
}

define i64 @mul64(i64 %a, i64 %b) nounwind {
; M32I-LABEL: mul64:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -8
; M32I-NEXT:    st r4, [sp, 4]
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 8
; M32I-NEXT:    movu r4, %hi(__muldi3)
; M32I-NEXT:    movl r4, %lo(__muldi3)
; M32I-NEXT:    rcall r4, 0
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    ld r4, [sp, 4]
; M32I-NEXT:    addi sp, sp, 8
; M32I-NEXT:    ret
  %1 = mul i64 %a, %b
  ret i64 %1
}

define i64 @mul64_constant(i64 %a) nounwind {
; M32I-LABEL: mul64_constant:
; M32I:       ; %bb.0:
; M32I-NEXT:    addi sp, sp, -8
; M32I-NEXT:    st r4, [sp, 4]
; M32I-NEXT:    st r14, [sp, 0]
; M32I-NEXT:    addi r14, sp, 8
; M32I-NEXT:    movu r4, %hi(__muldi3)
; M32I-NEXT:    movl r4, %lo(__muldi3)
; M32I-NEXT:    movi r2, 5
; M32I-NEXT:    movi r3, 0
; M32I-NEXT:    rcall r4, 0
; M32I-NEXT:    ld r14, [sp, 0]
; M32I-NEXT:    ld r4, [sp, 4]
; M32I-NEXT:    addi sp, sp, 8
; M32I-NEXT:    ret
  %1 = mul i64 %a, 5
  ret i64 %1
}
