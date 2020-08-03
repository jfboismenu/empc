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

#include <empc/cpu/cpu_state.h>
#include <empc/memory/memory.h>
#include <empc/cpu/modrm.h>

namespace empc {

class CPU {
public:
    CPU(Memory& memory);
    void emulate_once();
    void reset() noexcept;

    CPUState& state();
    const CPUState &state() const;

    bool is_halted() const;
    unsigned long long cpu_time() const;

private:
// =============
// Instructions
// =============

    template <typename Operand>
    void _jmp_absolute(Operand offset, Operand segment) noexcept;

    template <typename Operand>
    void _jmp_near(Operand ip_offset) noexcept;

    template <typename DataType>
    void _mov_imm(DataType& reg);
    template <typename DataType>
    void _mov_a2_a3(const DataType& data, address addr);
    template <typename DataType>
    void _mov_a0_a1(DataType &data, address addr);
    template <typename DataType>
    void _mov_88_89();
    template <typename DataType>
    void _mov_8a_8b();

    void _hlt();

    void _unknown_opcode(byte opcode) const;
    address _get_program_counter() const noexcept;
    // ================
    // Utility methods
    // ================
    template <typename DataType>
    DataType _fetch_operand() noexcept;

    template <typename DataType>
    DataType& _get_reg_from_modrm(const ModRMByte data);

    template <typename DataType>
    DataType& _get_rm_reg_from_modrm(const ModRMByte data);

    template <typename DataType>
    DataType _get_source_from_modrm(const ModRMByte data);

    word _get_rm_mem_from_modrm(const ModRMByte byte);

    word _data_segment() const;
    word _stack_segment() const;

    // =============
    // Data members
    // =============
    Memory &_memory;
    CPUState _state;
};

}
