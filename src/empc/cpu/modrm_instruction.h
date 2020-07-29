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

#include <empc/cpu/instruction.h>
#include <empc/cpu/imp/modrm_instruction.h>
#include <empc/cpu/cpu_state.h>

namespace empc {

template <typename IMPL, typename DATA_TYPE, bool LOCKABLE = false>
struct ModRMInstruction : public Instruction<IMPL, LOCKABLE>
{
    static DATA_TYPE &get_reg_from_modrm(CPUState &state, const ModRMByte data)
    {
        return imp::get_reg_from_modrm<DATA_TYPE>(state, data);
    }

    static DATA_TYPE &get_rm_reg_from_modrm(CPUState &state, const ModRMByte data)
    {
        return imp::get_rm_reg_from_modrm<DATA_TYPE>(state, data);
    }

    static DATA_TYPE get_source_from_modrm(CPUState &state, Memory &memory, const ModRMByte data)
    {
        return imp::get_source_from_modrm<DATA_TYPE>(state, memory, data);
    }

    static word get_rm_mem_from_modrm(CPUState &state, Memory &memory, const ModRMByte data)
    {
        return imp::get_rm_mem_from_modrm(state, memory, data);
    }

    static void _execute(CPUState &state, Memory &memory);
};

template <typename IMPL, typename DATA_TYPE, bool LOCKABLE>
void ModRMInstruction<IMPL, DATA_TYPE, LOCKABLE>::_execute(CPUState &state, Memory &memory)
{
    const ModRMByte modrm{imp::fetch_operand<byte>(state, memory)};

    if (modrm.bits.mode == 0)
    {
        if (modrm.bits.rm != 0b110)
        {
            throw std::runtime_error(fmt::format("Unsupported rm {:03b}", modrm.bits.rm));
        }

        const address addr{imp::fetch_operand<word>(state, memory)};
        const DATA_TYPE &operand{imp::get_reg_from_modrm<DATA_TYPE>(state, modrm)};
        const DATA_TYPE result{IMPL::_modrm_execute(state, memory, modrm, operand)};

        // Effective address of displacement cost is 6 cycles
        state.cpu_time += 6;
        memory.write(addr, result);
    }
    else if (modrm.bits.mode == 0b11)
    {
        imp::get_rm_reg_from_modrm<DATA_TYPE>(state, modrm) = IMPL::_modrm_execute(
            state, memory, modrm, imp::get_reg_from_modrm<DATA_TYPE>(state, modrm));
    }
    else
    {
        throw std::runtime_error(fmt::format("Unsupported mode {:02b}", modrm.bits.mode));
    }
}
}
