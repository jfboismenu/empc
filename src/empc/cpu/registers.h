
// MIT License
//
// Copyright (c) 2020 Jean-François Boismenu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <empc/base/types.h>

namespace empc {

#define EMPC_GP_REG_DECL(reg)   \
    const byte& reg##l() const; \
    byte& reg##l();             \
    const byte& reg##h() const; \
    byte& reg##h();             \
    const word& reg##x() const; \
    word& reg##x();

class GeneralPurposeRegisters {
public:
    GeneralPurposeRegisters();

    // Defines const not consts accessors for
    // registers combinations {a,b,c,d}{l,h,x}
    EMPC_GP_REG_DECL(a);
    EMPC_GP_REG_DECL(b);
    EMPC_GP_REG_DECL(c);
    EMPC_GP_REG_DECL(d);

private:
    union {
        struct
        {
            byte al;
            byte ah;
            byte bl;
            byte bh;
            byte cl;
            byte ch;
            byte dl;
            byte dh;
        } r8;
        struct
        {
            word ax;
            word bx;
            word cx;
            word dx;
        } r16;
    } _regs;
};

inline GeneralPurposeRegisters::GeneralPurposeRegisters()
    : _regs { .r16 = { 0, 0, 0, 0 } }
{
}

#define EMPC_GP_REG_IMP(reg)                                                                \
    inline const byte& GeneralPurposeRegisters::reg##l() const { return _regs.r8.reg##l; }  \
    inline byte& GeneralPurposeRegisters::reg##l() { return _regs.r8.reg##l; }              \
    inline const byte& GeneralPurposeRegisters::reg##h() const { return _regs.r8.reg##h; }  \
    inline byte& GeneralPurposeRegisters::reg##h() { return _regs.r8.reg##h; }              \
    inline const word& GeneralPurposeRegisters::reg##x() const { return _regs.r16.reg##x; } \
    inline word& GeneralPurposeRegisters::reg##x() { return _regs.r16.reg##x; }

EMPC_GP_REG_IMP(a);
EMPC_GP_REG_IMP(b);
EMPC_GP_REG_IMP(c);
EMPC_GP_REG_IMP(d);

union IndexRegisters {
    struct
    {
    public:
        word si;

    private:
        [[maybe_unused]] word __pad0;

    public:
        word di;

    private:
        [[maybe_unused]] word __pad1;

    public:
        word bp;

    private:
        [[maybe_unused]] word __pad2;

    public:
        word sp;

    private:
        [[maybe_unused]] word __pad3;
    } r16;
    struct
    {
        dword esi;
        dword edi;
        dword ebp;
        dword esp;
    } r32;
};

union InstructionPointer {
    word r16;
    dword r32;
};

struct SegmentSelectors {
public:
    word cs;
    word ds;
    word es;
    word ss;
};

struct Registers {
public:
    GeneralPurposeRegisters mr;
    IndexRegisters ir;
    InstructionPointer pc;
    SegmentSelectors ss;
};
}
