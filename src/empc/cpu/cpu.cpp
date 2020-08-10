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
#include <empc/memory/memory.imp.h>
#include <fmt/core.h>

#include <empc/cpu/instructions/register_accessor.h>

#include <empc/cpu/instructions/hlt.h>
#include <empc/cpu/instructions/jmp.h>
#include <empc/cpu/instructions/mov.h>
#include <empc/cpu/instructions/nop.h>

namespace empc {

CPU::CPU(Memory &memory) : _memory(memory) {
}

void CPU::reset() noexcept {
    // Source: The 8086 Family User's Manual, October 1979
    // Page 2-29, table 2-4.
    _state.reset();
}

CPUState &CPU::state() {
    return _state;
}

const CPUState &CPU::state() const {
    return _state;
}

unsigned long long CPU::cpu_time() const {
    return _state.cpu_time;
}

void CPU::emulate_once() {
    const byte opcode{imp::fetch_operand<byte>(_state, _memory)};
    switch (opcode) {
        case 0x90: {
            Nop::execute(_state, _memory);
        } break;
        case 0xF4: {
            Hlt::execute(_state, _memory);
        } break;
        case 0x88: {
            Mov8889<byte>::execute(_state, _memory);
        } break;
        case 0x89: {
            Mov8889<word>::execute(_state, _memory);
        } break;
        case 0x8a: {
            Mov8a8b<byte>::execute(_state, _memory);
        } break;
        case 0x8b: {
            Mov8a8b<word>::execute(_state, _memory);
        } break;
        case 0xA0: {
            MovA0A1<Register::AL>::execute(_state, _memory);
        } break;
        case 0xA1: {
            MovA0A1<Register::AX>::execute(_state, _memory);
        } break;
        case 0xA2: {
            MovA2A3::execute(_state, _memory, _state.al());
        } break;
        case 0xA3: {
            MovA2A3::execute(_state, _memory, _state.ax());
        } break;
        case 0xB0: {
            MovImm<Register::AL>::execute(_state, _memory);
        } break;
        case 0xB1: {
            MovImm<Register::CL>::execute(_state, _memory);
        } break;
        case 0xB2: {
            MovImm<Register::DL>::execute(_state, _memory);
        } break;
        case 0xB3: {
            MovImm<Register::BL>::execute(_state, _memory);
        } break;
        case 0xB4: {
            MovImm<Register::AH>::execute(_state, _memory);
        } break;
        case 0xB5: {
            MovImm<Register::CH>::execute(_state, _memory);
        } break;
        case 0xB6: {
            MovImm<Register::DH>::execute(_state, _memory);
        } break;
        case 0xB7: {
            MovImm<Register::BH>::execute(_state, _memory);
        } break;
        case 0xB8: {
            MovImm<Register::AX>::execute(_state, _memory);
        } break;
        case 0xB9: {
            MovImm<Register::CX>::execute(_state, _memory);
        } break;
        case 0xBA: {
            MovImm<Register::DX>::execute(_state, _memory);
        } break;
        case 0xBB: {
            MovImm<Register::BX>::execute(_state, _memory);
        } break;
        case 0xBC: {
            MovImm<Register::SP>::execute(_state, _memory);
        } break;
        case 0xBD: {
            MovImm<Register::BP>::execute(_state, _memory);
        } break;
        case 0xBE: {
            MovImm<Register::SI>::execute(_state, _memory);
        } break;
        case 0xBF: {
            MovImm<Register::DI>::execute(_state, _memory);
        } break;
        case 0xE9: {
            JmpNear::execute(_state, _memory);
        } break;
        case 0xEA: {
            JmpAbs::execute(_state, _memory);
        } break;
        default: {
            _unknown_opcode(opcode);
        } break;
    }
}

void CPU::_hlt() {
    _state.is_halted = true;
    _state.cpu_time += 2;
}

bool CPU::is_halted() const {
    return _state.is_halted;
}

void CPU::_unknown_opcode(byte opcode) const {
    throw std::runtime_error(fmt::format("Unexpected opcode {:02x}", opcode));
}

} // namespace empc
