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

#include <iostream>

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

void CPU::_mov_reg_to_memreg()
{
    byte modrm = _fetch_operand<byte>();

    if (modrm == 0x1E) {
        _memory.write(_fetch_operand<word>(), _dr.bx());
    }
    else {
        throw std::runtime_error(fmt::format("Unknown modrm {:02x}", modrm));
    }
}

}
