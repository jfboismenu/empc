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

namespace empc {

CPU::CPU(Memory& memory)
    : _memory(memory),
    _is_halted{false},
    _cpu_time{0}
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
    _is_halted = false;
}

unsigned long long CPU::cpu_time() const
{
    return _cpu_time;
}

void CPU::emulate_once()
{
    const byte opcode { _fetch_operand<byte>() };
    switch (opcode) {
    case 0x90: {
        // NOP!
        _cpu_time += 3;
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
        _mov_a0_a1(_dr.al(), _fetch_operand<word>());
    } break;
    case 0xA1: {
        _mov_a0_a1(_dr.ax(), _fetch_operand<word>());
    } break;
    case 0xA2: {
        _mov_a2_a3(_dr.al(), _fetch_operand<word>());
    } break;
    case 0xA3: {
        _mov_a2_a3(_dr.ax(), _fetch_operand<word>());
    } break;
    case 0xB0: {
        _mov_imm(_dr.al());
    } break;
    case 0xB1: {
        _mov_imm(_dr.cl());
    } break;
    case 0xB2: {
        _mov_imm(_dr.dl());
    } break;
    case 0xB3: {
        _mov_imm(_dr.bl());
    } break;
    case 0xB4: {
        _mov_imm(_dr.ah());
    } break;
    case 0xB5: {
        _mov_imm(_dr.ch());
    } break;
    case 0xB6: {
        _mov_imm(_dr.dh());
    } break;
    case 0xB7: {
        _mov_imm(_dr.bh());
    } break;
    case 0xB8: {
        _mov_imm(_dr.ax());
    } break;
    case 0xB9: {
        _mov_imm(_dr.cx());
    } break;
    case 0xBA: {
        _mov_imm(_dr.dx());
    } break;
    case 0xBB: {
        _mov_imm(_dr.bx());
    } break;
    case 0xBC: {
        _mov_imm(_pair.sp());
    } break;
    case 0xBD: {
        _mov_imm(_pair.bp());
    } break;
    case 0xBE: {
        _mov_imm(_pair.si());
    } break;
    case 0xBF: {
        _mov_imm(_pair.di());
    } break;
    case 0xE9: {
        _jmp_near(_fetch_operand<word>());
    } break;
    case 0xEA: {
        _jmp_absolute(_fetch_operand<word>(), _fetch_operand<word>());
    } break;
    default: {
        _unknown_opcode(opcode);
    } break;
    }
}

void CPU::_hlt()
{
    _is_halted = true;
    _cpu_time += 2;
}

bool CPU::is_halted() const
{
    return _is_halted;
}

void CPU::_unknown_opcode(byte opcode) const
{
    throw std::runtime_error(fmt::format("Unexpected opcode {:02x}", opcode));
}

template <typename DataType>
DataType CPU::_fetch_operand() noexcept
{
    const DataType result{_memory.read<DataType>(_get_program_counter())};
    _ip.add<DataType>();
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

DataRegisters &CPU::data_registers()
{
    return _dr;
}

SegmentRegisters &CPU::segment_registers()
{
    return _sr;
}

InstructionPointer &CPU::instruction_pointer_register()
{
    return _ip;
}

PointerAndIndexRegisters &CPU::pointer_and_index_registers()
{
    return _pair;
}

template<>
word& CPU::_get_reg_from_modrm(const ModRMByte data) {
    switch(data.bits.reg) {
    case 0: {
        return _dr.ax();
    }
    case 1: {
        return _dr.cx();
    }
    case 2: {
        return _dr.dx();
    }
    case 3: {
        return _dr.bx();
    }
    case 4: {
        return _pair.sp();
    }
    case 5: {
        return _pair.bp();
    }
    case 6: {
        return _pair.si();
    }
    case 7: {
        return _pair.di();
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
        return _dr.al();
    }
    case 1: {
        return _dr.cl();
    }
    case 2: {
        return _dr.dl();
    }
    case 3: {
        return _dr.bl();
    }
    case 4: {
        return _dr.ah();
    }
    case 5: {
        return _dr.ch();
    }
    case 6: {
        return _dr.dh();
    }
    case 7: {
        return _dr.bh();
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
        return _dr.ax();
    }
    case 1: {
        return _dr.cx();
    }
    case 2: {
        return _dr.dx();
    }
    case 3: {
        return _dr.bx();
    }
    case 4: {
        return _pair.sp();
    }
    case 5: {
        return _pair.bp();
    }
    case 6: {
        return _pair.si();
    }
    case 7: {
        return _pair.di();
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
        return _dr.al();
    }
    case 1: {
        return _dr.cl();
    }
    case 2: {
        return _dr.dl();
    }
    case 3: {
        return _dr.bl();
    }
    case 4: {
        return _dr.ah();
    }
    case 5: {
        return _dr.ch();
    }
    case 6: {
        return _dr.dh();
    }
    case 7: {
        return _dr.bh();
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
        return _data_segment() + _dr.bx() + _pair.si() + disp;
    }
    case 1: {
        return _data_segment() + _dr.bx() + _pair.di() + disp;
    }
    case 2: {
        return _stack_segment() + _pair.bp() + _pair.si() + disp;
    }
    case 3: {
        return _stack_segment() + _pair.sp() + _pair.di() + disp;
    }
    case 4: {
        return _data_segment() + _pair.si() + disp;
    }
    case 5: {
        return _data_segment() + _pair.di() + disp;
    }
    case 6: {
        return _stack_segment() + _pair.bp() + disp;
    }
    case 7: {
        return _stack_segment() + _dr.bx() + disp;
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
    return _sr.ds() << 0x4;
}

word CPU::_stack_segment() const
{
    // FIXME: When segment override operations are implemented,
    // this method should return the right segment.
    return _sr.ss() << 0x4;
}
}

#include <empc/cpu/cpu.jmp.hpp>
#include <empc/cpu/cpu.mov.hpp>
