
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

namespace empc {
using byte = unsigned char;
using word = unsigned short;
using dword = unsigned int;

union MainRegisters {
    struct
    {
    public:
        byte al;
        byte ah;

    private:
        [[maybe_unused]] word __pad0;

    public:
        byte bl;
        byte bh;

    private:
        [[maybe_unused]] word __pad1;

    public:
        byte cl;
        byte ch;

    private:
        [[maybe_unused]] word __pad2;

    public:
        byte dl;
        byte dh;

    private:
        [[maybe_unused]] word __pad3;
    } r8;
    struct
    {
    public:
        word ax;

    private:
        [[maybe_unused]] word __pad0;

    public:
        word bx;

    private:
        [[maybe_unused]] word __pad1;

    public:
        word cx;

    private:
        [[maybe_unused]] word __pad2;

    public:
        word dx;

    private:
        [[maybe_unused]] word __pad3;
    } r16;
    struct
    {
        dword eax;
        dword ebx;
        dword ecx;
        dword edx;
    } r32;
};

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

union ProgramCounter {
    word r16;
    dword r32;
};

struct SegmentSelectors {
public:
    word cs;
    word ds;
    word es;
    word fs;
    word gs;
    word ss;
};

struct Registers {
public:
    MainRegisters mr;
    IndexRegisters ir;
    ProgramCounter pc;
    SegmentSelectors ss;
};
}
