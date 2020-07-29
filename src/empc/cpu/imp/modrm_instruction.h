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

#include <empc/cpu/imp/instruction.h>

namespace empc { namespace imp {

template <typename DataType>
DataType &get_reg(CPUState &state, const ModRMByte data);

template <typename DataType>
DataType &get_reg_from_modrm_(CPUState &state, const ModRMByte data);

template <typename DataType>
DataType &get_rm_reg(CPUState &state, const ModRMByte data);

template <typename DataType>
DataType get_source(CPUState &state, Memory &memory, const ModRMByte data);

word get_rm_mem(CPUState &state, Memory &memory, const ModRMByte data);

word data_segment(CPUState &state)
{
    // FIXME: When segment override operations are implemented,
    // this method should return the right segment.
    return state.ds() << 0x4;
}

word stack_segment(CPUState &state)
{
    // FIXME: When segment override operations are implemented,
    // this method should return the right segment.
    return state.ss() << 0x4;
}

template <>
word &get_reg(CPUState &state, const ModRMByte data)
{
    switch (data.bits.reg)
    {
    case 0:
    {
        return state.ax();
    }
    case 1:
    {
        return state.cx();
    }
    case 2:
    {
        return state.dx();
    }
    case 3:
    {
        return state.bx();
    }
    case 4:
    {
        return state.sp();
    }
    case 5:
    {
        return state.bp();
    }
    case 6:
    {
        return state.si();
    }
    case 7:
    {
        return state.di();
    }
    default:
    {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template <>
byte &get_reg(CPUState& state, const ModRMByte data)
{
    switch (data.bits.reg)
    {
    case 0:
    {
        return state.al();
    }
    case 1:
    {
        return state.cl();
    }
    case 2:
    {
        return state.dl();
    }
    case 3:
    {
        return state.bl();
    }
    case 4:
    {
        return state.ah();
    }
    case 5:
    {
        return state.ch();
    }
    case 6:
    {
        return state.dh();
    }
    case 7:
    {
        return state.bh();
    }
    default:
    {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template <>
word &get_rm_reg(CPUState& state, const ModRMByte data)
{
    switch (data.bits.rm)
    {
    case 0:
    {
        return state.ax();
    }
    case 1:
    {
        return state.cx();
    }
    case 2:
    {
        return state.dx();
    }
    case 3:
    {
        return state.bx();
    }
    case 4:
    {
        return state.sp();
    }
    case 5:
    {
        return state.bp();
    }
    case 6:
    {
        return state.si();
    }
    case 7:
    {
        return state.di();
    }
    default:
    {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template <>
byte &get_rm_reg(CPUState& state, const ModRMByte data)
{
    switch (data.bits.rm)
    {
    case 0:
    {
        return state.al();
    }
    case 1:
    {
        return state.cl();
    }
    case 2:
    {
        return state.dl();
    }
    case 3:
    {
        return state.bl();
    }
    case 4:
    {
        return state.ah();
    }
    case 5:
    {
        return state.ch();
    }
    case 6:
    {
        return state.dh();
    }
    case 7:
    {
        return state.bh();
    }
    default:
    {
        throw std::runtime_error("Unknown reg byte");
    }
    }
}


// if mod = 00 then DISP = 0*, disp-Iow and disp-high are absent
// if mod = 01 then DISP = disp-Iow sign-extended to 16 bits, disp-high is absent
// if mod = 10 then DISP = disp-high:disp-Iow
// if r/m = 000 then EA = (BX) + (SI) + DISP
// if r/m = 001 then EA = (BX) + (DI) + DISP
// if r/m = 010 then EA = (BP) + (SI) + DISP
// if r/m = 011 then EA = (BP) + (DI) + DISP
// if r/m = 100 then EA = (SI) + DISP
// if r/m = 101 then EA = (DI) + DISP
// if r/m = 110 then EA = (BP) + DISP*
// if r/m = 111 then EA = (BX) + DISP

word get_rm_mem(CPUState &state, Memory &memory, const ModRMByte data)
{
    word disp;
    if (data.bits.mode == 0)
    {
        if (data.bits.rm == 0b110)
        {
            state.cpu_time += 6;
            return fetch_operand<word>(state, memory);
        }
        else
        {
            disp = 0;
        }
    }
    else if (data.bits.mode == 0b01)
    {
        // When displacement is happening, we take a 4 cycle hit
        state.cpu_time += 4;
        disp = static_cast<word>(
            // We sign extend the 8bit value, so read as a char,
            // which goes from -128 to 127 and extend the sign to 16
            // bit.
            fetch_operand<char>(state, memory)
            );
    }
    else
    {
        // When displacement is happening, we take a 4 cycle hit
        state.cpu_time += 4;
        disp = fetch_operand<word>(state, memory);
    }

    switch (data.bits.rm)
    {
    case 0:
    {
        state.cpu_time += 7;
        return data_segment(state) + state.bx() + state.si() + disp;
    }
    case 1:
    {
        state.cpu_time += 7;
        return data_segment(state) + state.bx() + state.di() + disp;
    }
    case 2:
    {
        state.cpu_time += 7;
        return stack_segment(state) + state.bp() + state.si() + disp;
    }
    case 3:
    {
        state.cpu_time += 7;
        return stack_segment(state) + state.sp() + state.di() + disp;
    }
    case 4:
    {
        state.cpu_time += 5;
        return data_segment(state) + state.si() + disp;
    }
    case 5:
    {
        state.cpu_time += 5;
        return data_segment(state) + state.di() + disp;
    }
    case 6:
    {
        state.cpu_time += 5;
        return stack_segment(state) + state.bp() + disp;
    }
    case 7:
    {
        state.cpu_time += 5;
        return stack_segment(state) + state.bx() + disp;
    }
    default:
    {
        throw std::runtime_error("Unexpected rm byte");
    }
    }
}

template <typename DataType>
DataType get_source(CPUState& state, Memory& memory, const ModRMByte data)
{
    // The source is a register.
    if (data.bits.mode == 0b11)
    {
        // Read the register from the modrm byte.
        return imp::get_rm_reg<DataType>(state, data);
    }
    else
    {
        // Read the memory associated with the modrm byte.
        return memory.read<DataType>(imp::get_rm_mem(state, memory, data));
    }
}

}}
