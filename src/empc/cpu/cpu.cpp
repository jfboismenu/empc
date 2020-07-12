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

#include <empc/cpu/cpu.h>
#include <empc/cpu/cpu.jmp.hpp>
#include <empc/memory/memory.imp.h>
#include <fmt/core.h>

namespace empc {

CPU::CPU(Memory& memory)
    : _memory(memory)
{
}

void CPU::reset() noexcept
{
    // Source: The 8086 Family User's Manual, October 1979
    // Page 2-29, table 2-4.

    // FIXME: Empty flags
    _sr.cs() = 0xFFFF;
    _ip.ip() = 0;
    _sr.ds() = 0;
    _sr.es() = 0;
    _sr.ss() = 0;
}

void CPU::emulate_once()
{
    const byte opcode { _read_instruction_byte() };
    switch (opcode) {
    case 0xEA: {
        _jmp_absolute(_read_instruction_word(), _read_instruction_word());
    } break;
    default: {
        _unknown_opcode(opcode);
    } break;
    }
}

void CPU::_unknown_opcode(byte opcode) const
{
    throw std::runtime_error(fmt::format("Unexpected opcode {:02x}", opcode));
}

byte CPU::_read_instruction_byte() noexcept
{
    const byte result { _memory.read_byte(_get_program_counter()) };
    _ip.add<byte>();
    return result;
}

word CPU::_read_instruction_word() noexcept
{
    const word result { _memory.read_word(_get_program_counter()) };
    _ip.add<word>();
    return result;
}

address CPU::_get_program_counter() const noexcept
{
    return (_sr.cs() << 0x4) + _ip.ip();
}

const DataRegisters& CPU::data_registers() const
{
    return _dr;
}

const SegmentRegisters& CPU::segment_registers() const
{
    return _sr;
}

const InstructionPointer &CPU::instruction_pointer_register() const
{
    return _ip;
}

const PointerAndIndexRegisters &CPU::pointer_and_index_registers() const
{
    return _pair;
}


}
