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

union MainRegisters {
    struct
    {
    public:
        unsigned char al;
        unsigned char ah;

    private:
        short _pad0;

    public:
        unsigned char bl;
        unsigned char bh;

    private:
        short _pad1;

    public:
        unsigned char cl;
        unsigned char ch;

    private:
        short _pad2;

    public:
        unsigned char dl;
        unsigned char dh;

    private:
        short _pad3;
    } r8;
    struct
    {
    public:
        short ax;

    private:
        short _pad0;

    public:
        short bx;

    private:
        short _pad1;

    public:
        short cx;

    private:
        short _pad2;

    public:
        short dx;

    private:
        short _pad3;
    } r16;
    struct
    {
        int eax;
        int ebx;
        int ecx;
        int edx;
    } r32;
};

union IndexRegisters {
    struct
    {
    public:
        short si;

    private:
        short _pad0;

    public:
        short di;

    private:
        short _pad1;

    public:
        short bp;

    private:
        short _pad2;

    public:
        short sp;

    private:
        short _pad3;
    } r16;
    struct
    {
        int esi;
        int edi;
        int ebp;
        int esp;
    } r32;
};

union ProgramCounter {
    short r16;
    int r32;
};

struct SegmentSelectors {
public:
    short cs;
    short ds;
    short es;
    short fs;
    short gs;
    short ss;
};

struct Registers {
public:
    MainRegisters mr;
    IndexRegisters ir;
    ProgramCounter pc;
    SegmentSelectors ss;
};

class CPU {
private:
    Registers _regs;
};

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
push_byte(unsigned char byte)
{
}

extern "C" void
push_word(unsigned short word)
{
}

extern "C" unsigned char
pop_byte()
{
    return 0;
}

extern "C" unsigned short
pop_word()
{
    return 0;
}

extern "C" void
call(unsigned int addr)
{
    // Execute until the program counter is set to the return value
}

extern "C" unsigned char
read_byte(unsigned int addr)
{
    return 0;
}

extern "C" unsigned short
read_word(unsigned int addr)
{
    return 0;
}
