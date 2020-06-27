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

#include <cpu/registers.h>

namespace {
empc::Registers regs {};
}

namespace empc {

// resets regs, including sp and bp.
extern "C" void
reset()
{
}

extern "C" void
write_memory(int start, void* data, int size)
{
}

extern "C" void
read_registers(
    // Main registers
    dword* eax,
    dword* ebx,
    dword* ecx,
    dword* edx,
    // Index registers
    dword* esi,
    dword* edi,
    dword* ebp,
    dword* esp,
    // Program counter
    dword* pc,
    // Segment selectors
    word* cs,
    word* ds,
    word* es,
    word* fs,
    word* gs,
    word* ss)
{
    *eax = regs.mr.r32.eax;
    *ebx = regs.mr.r32.ebx;
    *ecx = regs.mr.r32.ecx;
    *edx = regs.mr.r32.edx;

    *esi = regs.ir.r32.esi;
    *edi = regs.ir.r32.edi;
    *ebp = regs.ir.r32.ebp;
    *esp = regs.ir.r32.esp;

    *pc = regs.pc.r32;

    *cs = regs.ss.cs;
    *ds = regs.ss.ds;
    *es = regs.ss.es;
    *fs = regs.ss.fs;
    *gs = regs.ss.gs;
    *ss = regs.ss.ss;
}

template <typename T>
void write_if_set(T* src, T& dst)
{
    if (src != nullptr) {
        dst = *src;
    }
}
extern "C" void
write_registers(
    // Main registers
    dword* eax,
    dword* ebx,
    dword* ecx,
    dword* edx,
    // Index registers
    dword* esi,
    dword* edi,
    dword* ebp,
    dword* esp,
    // Program counter
    dword* pc,
    // Segment selectors
    word* cs,
    word* ds,
    word* es,
    word* fs,
    word* gs,
    word* ss)
{
    write_if_set(eax, regs.mr.r32.eax);
    write_if_set(ebx, regs.mr.r32.ebx);
    write_if_set(ecx, regs.mr.r32.ecx);
    write_if_set(edx, regs.mr.r32.edx);
    write_if_set(esi, regs.ir.r32.esi);
    write_if_set(edi, regs.ir.r32.edi);
    write_if_set(ebp, regs.ir.r32.ebp);
    write_if_set(esp, regs.ir.r32.esp);
    write_if_set(pc, regs.pc.r32);
    write_if_set(cs, regs.ss.cs);
    write_if_set(ds, regs.ss.ds);
    write_if_set(es, regs.ss.es);
    write_if_set(fs, regs.ss.fs);
    write_if_set(gs, regs.ss.gs);
    write_if_set(ss, regs.ss.ss);
}

extern "C" void
emulate_once()
{
}

}
