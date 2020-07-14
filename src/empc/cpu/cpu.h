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

#include <empc/cpu/data_registers.h>
#include <empc/cpu/instruction_pointer.h>
#include <empc/cpu/pointer_and_index_registers.h>
#include <empc/cpu/segment_registers.h>
#include <empc/memory/memory.h>

namespace empc {

class CPU {
public:
    CPU(Memory& memory);
    void emulate_once();
    void reset() noexcept;

    const DataRegisters& data_registers() const;
    const SegmentRegisters& segment_registers() const;
    const InstructionPointer &instruction_pointer_register() const;
    const PointerAndIndexRegisters &pointer_and_index_registers() const;

private:
// =============
// Instructions
// =============

    template <typename Operand>
    void _jmp_absolute(Operand offset, Operand segment) noexcept;

    template <typename Operand>
    void _jmp_near(Operand ip_offset) noexcept;

    void _unknown_opcode(byte opcode) const;
    address _get_program_counter() const noexcept;
    // ================
    // Utility methods
    // ================
    byte _read_instruction_byte() noexcept;
    word _read_instruction_word() noexcept;

// =============
// Data members
// =============
    Memory& _memory;

    DataRegisters _dr;
    PointerAndIndexRegisters _pair;
    InstructionPointer _ip;
    SegmentRegisters _sr;
};

}
