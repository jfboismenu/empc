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
#include <empc/cpu/instruction.h>
#include <empc/cpu/modrm.h>

namespace empc {

struct MovImm : public Instruction<MovImm> {
    template <typename DataType>
    static void _execute(CPUState &state, Memory &memory, DataType &reg) {
        reg = fetch_operand<DataType>(state, memory);
        state.cpu_time += 4;
    }
};

struct MovA2A3 : public Instruction<MovA2A3> {
    template <typename DataType>
    static void _execute(CPUState &state, Memory &memory, const DataType &data) {
        memory.write(fetch_operand<word>(state, memory), data);
        state.cpu_time += 10;
    }
};

struct MovA0A1 : public Instruction<MovA0A1> {
    template <typename DataType>
    static void _execute(CPUState &state, Memory &memory, DataType &reg) {
        reg = memory.read<DataType>(fetch_operand<word>(state, memory));
        state.cpu_time += 10;
    }
};

template <typename DATA_TYPE> struct Mov8889 : public Instruction<Mov8889<DATA_TYPE>> {
    static void _execute(CPUState &state, Memory &memory) {
        auto modrm = ModRM::decode(state, memory);
        modrm.write_rm(state, memory, modrm.read_reg<DATA_TYPE>(state));
        if (modrm.modrm_byte().is_rm_reg()) {
            state.cpu_time += 2;
        } else {
            state.cpu_time += 9;
        }
    }
};

template <typename DATA_TYPE> struct Mov8a8b : public Instruction<Mov8a8b<DATA_TYPE>> {
    static void _execute(CPUState &state, Memory &memory) {
        auto modrm = ModRM::decode(state, memory);
        modrm.write_reg(state, modrm.read_rm_mem<DATA_TYPE>(memory));
        state.cpu_time += 8;
    }
};

} // namespace empc
