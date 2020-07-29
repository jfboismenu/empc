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

#include <empc/cpu/instruction.h>
#include <empc/cpu/modrm_instruction.h>

namespace empc {

struct MovImm : public Instruction<MovImm>
{
    template<typename DataType>
    static void _execute(CPUState& state, Memory& memory, DataType& reg) {
        reg = fetch_operand<DataType>(state, memory);
        state.cpu_time += 4;
    }
};

struct MovA2A3 : public Instruction<MovA2A3>
{
    template <typename DataType>
    static void _execute(CPUState &state, Memory &memory, const DataType &data)
    {
        memory.write(fetch_operand<word>(state, memory), data);
        state.cpu_time += 10;
    }
};

struct MovA0A1 : public Instruction<MovA0A1>
{
    template <typename DataType>
    static void _execute(CPUState &state, Memory &memory, DataType &reg)
    {
        reg = memory.read<DataType>(fetch_operand<word>(state, memory));
        state.cpu_time += 10;
    }
};

template<typename DataType>
struct Mov8889 : public ModRMInstruction<Mov8889<DataType> >
{
    using ThisClass = Mov8889<DataType>;
    static void _execute(CPUState &state, Memory &memory)
    {
        const ModRMByte modrm{imp::fetch_operand<byte>(state, memory)};

        if (modrm.bits.mode == 0) {
            // Base instruction if 9 cycles
            state.cpu_time += 9;
            if (modrm.bits.rm != 0b110) {
                throw std::runtime_error(fmt::format("Unsupported rm {:03b}", modrm.bits.rm));
            }

            // Effective address of displacement cost is 6 cycles
            state.cpu_time += 6;
            memory.write(imp::fetch_operand<word>(state, memory),
                         imp::get_reg_from_modrm<DataType>(state, modrm));
        } else if (modrm.bits.mode == 0b11) {
            imp::get_rm_reg_from_modrm<DataType>(state, modrm) = imp::get_reg_from_modrm<DataType>(
                state, modrm);
            state.cpu_time += 2;
        } else {
            throw std::runtime_error(fmt::format("Unsupported mode {:02b}", modrm.bits.mode));
        }
    }
};

template <typename DataType>
void CPU::_mov_8a_8b()
{
    const ModRMByte modrm{_fetch_operand<byte>()};
    _state.cpu_time += 8;
    _get_reg_from_modrm<DataType>(modrm) = _get_source_from_modrm<DataType>(modrm);
}

}
