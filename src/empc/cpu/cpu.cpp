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


#include <empc/cpu/cpu.jmp.hpp>
#include <empc/cpu/cpu.mov.hpp>


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
    _state.reset();
}

CPUState &CPU::state()
{
    return _state;
}

const CPUState &CPU::state() const
{
    return _state;
}

unsigned long long CPU::cpu_time() const
{
    return _state.cpu_time;
}

void CPU::emulate_once()
{
    const byte opcode { _fetch_operand<byte>() };
    switch (opcode) {
    case 0x90: {
        // NOP!
        _state.cpu_time += 3;
    };
    case 0xF4: {
        _hlt();
    } break;
    case 0x88: {
        _mov_88_89<byte>();
    } break;
    case 0x89: {
        _mov_88_89<word>();
    } break;
    case 0x8a: {
        _mov_8a_8b<byte>();
    } break;
    case 0x8b: {
        _mov_8a_8b<word>();
    } break;
    case 0xA0: {
        MovA0A1::execute(_state, _memory, _state.al());
    } break;
    case 0xA1: {
        MovA0A1::execute(_state, _memory, _state.ax());
    } break;
    case 0xA2: {
        MovA2A3::execute(_state, _memory, _state.al());
    } break;
    case 0xA3: {
        MovA2A3::execute(_state, _memory, _state.ax());
    } break;
    case 0xB0: {
        MovImm::execute(_state, _memory, _state.al());
    } break;
    case 0xB1: {
        MovImm::execute(_state, _memory, _state.cl());
    } break;
    case 0xB2: {
        MovImm::execute(_state, _memory, _state.dl());
    } break;
    case 0xB3: {
        MovImm::execute(_state, _memory, _state.bl());
    } break;
    case 0xB4: {
        MovImm::execute(_state, _memory, _state.ah());
    } break;
    case 0xB5: {
        MovImm::execute(_state, _memory, _state.ch());
    } break;
    case 0xB6: {
        MovImm::execute(_state, _memory, _state.dh());
    } break;
    case 0xB7: {
        MovImm::execute(_state, _memory, _state.bh());
    } break;
    case 0xB8: {
        MovImm::execute(_state, _memory, _state.ax());
    } break;
    case 0xB9: {
        MovImm::execute(_state, _memory, _state.cx());
    } break;
    case 0xBA: {
        MovImm::execute(_state, _memory, _state.dx());
    } break;
    case 0xBB: {
        MovImm::execute(_state, _memory, _state.bx());
    } break;
    case 0xBC: {
        MovImm::execute(_state, _memory, _state.sp());
    } break;
    case 0xBD: {
        MovImm::execute(_state, _memory, _state.bp());
    } break;
    case 0xBE: {
        MovImm::execute(_state, _memory, _state.si());
    } break;
    case 0xBF: {
        MovImm::execute(_state, _memory, _state.di());
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

void CPU::_hlt()
{
    _state.is_halted = true;
    _state.cpu_time += 2;
}

bool CPU::is_halted() const
{
    return _state.is_halted;
}

void CPU::_unknown_opcode(byte opcode) const
{
    throw std::runtime_error(fmt::format("Unexpected opcode {:02x}", opcode));
}

template <typename DataType>
DataType CPU::_fetch_operand() noexcept
{
    const DataType result{_memory.read<DataType>(_get_program_counter())};
    _state.ip() += sizeof(DataType);
    return result;
}
address CPU::_get_program_counter() const noexcept
{
    return (_state.cs() << 0x4) + _state.ip();
}
template<>
word& CPU::_get_reg_from_modrm(const ModRMByte data) {
    switch(data.bits.reg) {
    case 0: {
        return _state.ax();
    }
    case 1: {
        return _state.cx();
    }
    case 2: {
        return _state.dx();
    }
    case 3: {
        return _state.bx();
    }
    case 4: {
        return _state.sp();
    }
    case 5: {
        return _state.bp();
    }
    case 6: {
        return _state.si();
    }
    case 7: {
        return _state.di();
    }
    default: {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template<>
byte& CPU::_get_reg_from_modrm(const ModRMByte data) {
    switch(data.bits.reg) {
    case 0: {
        return _state.al();
    }
    case 1: {
        return _state.cl();
    }
    case 2: {
        return _state.dl();
    }
    case 3: {
        return _state.bl();
    }
    case 4: {
        return _state.ah();
    }
    case 5: {
        return _state.ch();
    }
    case 6: {
        return _state.dh();
    }
    case 7: {
        return _state.bh();
    }
    default: {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template<>
word& CPU::_get_rm_reg_from_modrm(const ModRMByte data) {
    switch(data.bits.rm) {
    case 0: {
        return _state.ax();
    }
    case 1: {
        return _state.cx();
    }
    case 2: {
        return _state.dx();
    }
    case 3: {
        return _state.bx();
    }
    case 4: {
        return _state.sp();
    }
    case 5: {
        return _state.bp();
    }
    case 6: {
        return _state.si();
    }
    case 7: {
        return _state.di();
    }
    default: {
        throw std::runtime_error("Unknown reg");
    }
    }
}

template<>
byte& CPU::_get_rm_reg_from_modrm(const ModRMByte data) {
    switch(data.bits.rm) {
    case 0: {
        return _state.al();
    }
    case 1: {
        return _state.cl();
    }
    case 2: {
        return _state.dl();
    }
    case 3: {
        return _state.bl();
    }
    case 4: {
        return _state.ah();
    }
    case 5: {
        return _state.ch();
    }
    case 6: {
        return _state.dh();
    }
    case 7: {
        return _state.bh();
    }
    default: {
        throw std::runtime_error("Unknown reg byte");
    }
    }
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

word CPU::_get_rm_mem_from_modrm(const ModRMByte data)
{
    word disp;
    if (data.bits.mode == 0) {
        if (data.bits.rm == 0b110)
        {
            return _fetch_operand<word>();
        }
        else {
            disp = 0;
        }
    }
    else if (data.bits.mode == 0b01)
    {
        // Sign extended to word.
        disp = static_cast<word>(_fetch_operand<byte>());
    }
    else
    {
        disp = _fetch_operand<word>();
    }

    switch(data.bits.rm) {
    case 0: {
        return _data_segment() + _state.bx() + _state.si() + disp;
    }
    case 1: {
        return _data_segment() + _state.bx() + _state.di() + disp;
    }
    case 2: {
        return _stack_segment() + _state.bp() + _state.si() + disp;
    }
    case 3: {
        return _stack_segment() + _state.sp() + _state.di() + disp;
    }
    case 4: {
        return _data_segment() + _state.si() + disp;
    }
    case 5: {
        return _data_segment() + _state.di() + disp;
    }
    case 6: {
        return _stack_segment() + _state.bp() + disp;
    }
    case 7: {
        return _stack_segment() + _state.bx() + disp;
    }
    default: {
        throw std::runtime_error("Unexpected rm byte");
    }
    }
}

template <typename DataType>
DataType CPU::_get_source_from_modrm(const ModRMByte data)
{
    // The source is a register.
    if (data.bits.mode == 0b11) {
        // Read the register from the modrm byte.
        return _get_rm_reg_from_modrm<DataType>(data);
    }
    else {
        // Read the memory associated with the modrm byte.
        return _memory.read<DataType>(_get_rm_mem_from_modrm(data));
    }
}

word CPU::_data_segment() const
{
    // FIXME: When segment override operations are implemented,
    // this method should return the right segment.
    return _state.ds() << 0x4;
}

word CPU::_stack_segment() const
{
    // FIXME: When segment override operations are implemented,
    // this method should return the right segment.
    return _state.ss() << 0x4;
}
}
