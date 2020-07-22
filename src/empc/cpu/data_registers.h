
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

class DataRegisters {
public:
    DataRegisters();
// Defines const not consts accessors for
// registers combinations {a,b,c,d}{l,h,x}



};

inline DataRegisters::DataRegisters()
    : _regs {.r16 = { 0, 0, 0, 0 } }
{
}

inline const byte& DataRegisters::al() const
{
    return _regs.r8.al;
}
inline byte& DataRegisters::al()
{
    return _regs.r8.al;
}
inline const byte& DataRegisters::ah() const
{
    return _regs.r8.ah;
}
inline byte& DataRegisters::ah()
{
    return _regs.r8.ah;
}
inline const word& DataRegisters::ax() const
{
    return _regs.r16.ax;
}
inline word& DataRegisters::ax()
{
    return _regs.r16.ax;
}
inline const byte& DataRegisters::bl() const
{
    return _regs.r8.bl;
}
inline byte& DataRegisters::bl()
{
    return _regs.r8.bl;
}
inline const byte& DataRegisters::bh() const
{
    return _regs.r8.bh;
}
inline byte& DataRegisters::bh()
{
    return _regs.r8.bh;
}
inline const word& DataRegisters::bx() const
{
    return _regs.r16.bx;
}
inline word& DataRegisters::bx()
{
    return _regs.r16.bx;
}
inline const byte& DataRegisters::cl() const
{
    return _regs.r8.cl;
}
inline byte& DataRegisters::cl()
{
    return _regs.r8.cl;
}
inline const byte& DataRegisters::ch() const
{
    return _regs.r8.ch;
}
inline byte& DataRegisters::ch()
{
    return _regs.r8.ch;
}
inline const word& DataRegisters::cx() const
{
    return _regs.r16.cx;
}
inline word& DataRegisters::cx()
{
    return _regs.r16.cx;
}
inline const byte& DataRegisters::dl() const
{
    return _regs.r8.dl;
}
inline byte& DataRegisters::dl()
{
    return _regs.r8.dl;
}
inline const byte& DataRegisters::dh() const
{
    return _regs.r8.dh;
}
inline byte& DataRegisters::dh()
{
    return _regs.r8.dh;
}
inline const word& DataRegisters::dx() const
{
    return _regs.r16.dx;
}
inline word& DataRegisters::dx()
{
    return _regs.r16.dx;
}
}
