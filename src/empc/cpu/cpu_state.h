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

struct CPUState
{

    CPUState();
    CPUState(const CPUState &) = delete;
    void reset();

    const byte &al() const;
    byte &al();
    const byte &ah() const;
    byte &ah();
    const word &ax() const;
    word &ax();

    const byte &bl() const;
    byte &bl();
    const byte &bh() const;
    byte &bh();
    const word &bx() const;
    word &bx();

    const byte &cl() const;
    byte &cl();
    const byte &ch() const;
    byte &ch();
    const word &cx() const;
    word &cx();

    const byte &dl() const;
    byte &dl();
    const byte &dh() const;
    byte &dh();
    const word &dx() const;
    word &dx();

    ;
    const word &di() const;
    word &di();
    const word &si() const;
    word &si();
    const word &bp() const;
    word &bp();
    const word &sp() const;
    word &sp();

    const word &cs() const;
    word &cs();
    const word &ds() const;
    word &ds();
    const word &es() const;
    word &es();
    const word &ss() const;
    word &ss();

    const word &ip() const;
    word &ip();

    address get_program_counter() const noexcept;

    unsigned long long cpu_time;
    bool is_halted;
    bool is_locked;

private:
    union
    {
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
    } _data_regs;

    word _si;
    word _di;
    word _bp;
    word _sp;

    word _cs;
    word _ds;
    word _es;
    word _ss;

    word _ip;
};

inline CPUState::CPUState()
    : cpu_time{0}, is_halted{false},
    is_locked{false},
    _data_regs{.r16 = {0, 0, 0, 0}},
    _si{0}, _di{0}, _bp{0}, _sp{0},
    _cs{0}, _ds{0}, _es{0}, _ss{0}
{
}

inline void CPUState::reset()
{
    _cs = 0xFFFF;
    _ip = 0;
    _ds = 0;
    _es = 0;
    _ss = 0;
    is_halted = false;
    cpu_time = 0;
}

inline const byte &CPUState::al() const
{
    return _data_regs.r8.al;
}
inline byte &CPUState::al()
{
    return _data_regs.r8.al;
}
inline const byte &CPUState::ah() const
{
    return _data_regs.r8.ah;
}
inline byte &CPUState::ah()
{
    return _data_regs.r8.ah;
}
inline const word &CPUState::ax() const
{
    return _data_regs.r16.ax;
}
inline word &CPUState::ax()
{
    return _data_regs.r16.ax;
}
inline const byte &CPUState::bl() const
{
    return _data_regs.r8.bl;
}
inline byte &CPUState::bl()
{
    return _data_regs.r8.bl;
}
inline const byte &CPUState::bh() const
{
    return _data_regs.r8.bh;
}
inline byte &CPUState::bh()
{
    return _data_regs.r8.bh;
}
inline const word &CPUState::bx() const
{
    return _data_regs.r16.bx;
}
inline word &CPUState::bx()
{
    return _data_regs.r16.bx;
}
inline const byte &CPUState::cl() const
{
    return _data_regs.r8.cl;
}
inline byte &CPUState::cl()
{
    return _data_regs.r8.cl;
}
inline const byte &CPUState::ch() const
{
    return _data_regs.r8.ch;
}
inline byte &CPUState::ch()
{
    return _data_regs.r8.ch;
}
inline const word &CPUState::cx() const
{
    return _data_regs.r16.cx;
}
inline word &CPUState::cx()
{
    return _data_regs.r16.cx;
}
inline const byte &CPUState::dl() const
{
    return _data_regs.r8.dl;
}
inline byte &CPUState::dl()
{
    return _data_regs.r8.dl;
}
inline const byte &CPUState::dh() const
{
    return _data_regs.r8.dh;
}
inline byte &CPUState::dh()
{
    return _data_regs.r8.dh;
}
inline const word &CPUState::dx() const
{
    return _data_regs.r16.dx;
}
inline word &CPUState::dx()
{
    return _data_regs.r16.dx;
}

inline const word &CPUState::si() const
{
    return _si;
}
inline const word &CPUState::di() const
{
    return _di;
}
inline const word &CPUState::bp() const
{
    return _bp;
}
inline const word &CPUState::sp() const
{
    return _sp;
}

inline word &CPUState::si()
{
    return _si;
}
inline word &CPUState::di()
{
    return _di;
}
inline word &CPUState::bp()
{
    return _bp;
}
inline word &CPUState::sp()
{
    return _sp;
}

inline const word &CPUState::cs() const
{
    return _cs;
}
inline word &CPUState::cs()
{
    return _cs;
}
inline const word &CPUState::ds() const
{
    return _ds;
}
inline word &CPUState::ds()
{
    return _ds;
}
inline const word &CPUState::es() const
{
    return _es;
}
inline word &CPUState::es()
{
    return _es;
}
inline const word &CPUState::ss() const
{
    return _ss;
}
inline word &CPUState::ss()
{
    return _ss;
}

inline const word &CPUState::ip() const
{
    return _ip;
}

inline word &CPUState::ip()
{
    return _ip;
}

inline address CPUState::get_program_counter() const noexcept
{
    return (cs() << 0x4) + ip();
}

}
