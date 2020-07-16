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

template <typename DataType>
void CPU::_mov_imm(DataType &reg)
{
    reg = _fetch_operand<DataType>();
}

template <typename DataType>
void CPU::_mov_reg_to_mem(const DataType &data, address addr)
{
    _memory.write_word(addr, data);
}

template <typename DataType>
void CPU::_mov_mem_to_reg(DataType &data, address addr)
{
    data = _memory.read<word>(addr);
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

#include <iostream>

template<typename DataType>
void CPU::_mov_88_89()
{
    const ModRMByte modrm{_fetch_operand<byte>()};

    if (modrm.bits.mode == 0) {
        if (modrm.bits.rm != 0b110) {
            throw std::runtime_error(fmt::format("Unsupported rm {:03b}", modrm.bits.rm));
        }
        _memory.write(_fetch_operand<word>(), _get_reg_from_modrm<DataType>(modrm));
    } else if (modrm.bits.mode == 0b11) {
        _get_rm_reg_from_modrm<DataType>(modrm) = _get_reg_from_modrm<DataType>(modrm);
    } else {
        throw std::runtime_error(fmt::format("Unsupported mode {:02b}", modrm.bits.mode));
    }


}

template <typename DataType>
void CPU::_mov_8a_8b()
{
    const ModRMByte modrm{_fetch_operand<byte>()};

    if (modrm.bits.mode != 0) {
        throw std::runtime_error(fmt::format("Unsupported mode {:02b}", modrm.bits.mode));
    }

    if (modrm.bits.rm != 0b110) {
        throw std::runtime_error(fmt::format("Unsupported rm {:03b}", modrm.bits.rm));
    }

    _get_reg_from_modrm<DataType>(modrm) = _memory.read<DataType>(_fetch_operand<word>());
}

}
