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

template <typename Operand>
void CPU::_jmp_absolute(Operand offset, Operand segment) noexcept
{
    static_assert(std::is_same<Operand, word>(), "Unsupported operand type.");
    _sr.cs() = segment;
    _ip.ip() = offset;
}

template <typename Operand>
void CPU::_jmp_near(Operand ip_offset) noexcept
{
    static_assert(std::is_same<Operand, word>(), "Unsupported operand type.");
    _ip.ip() += ip_offset;
}

}
