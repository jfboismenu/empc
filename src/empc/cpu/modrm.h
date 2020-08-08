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

#include <empc/base/types.h>
#include <empc/cpu/cpu_state.h>
#include <empc/cpu/imp/instruction.h>

#include <fmt/core.h>

#include <exception>

namespace empc {

struct ModRMByte {
    ModRMByte(byte data) : full(data) {
    }
    union {
        struct {
            // Bit fields are defined from lsb to msb.
            byte rm : 3;
            byte reg : 3;
            byte mode : 2;
        } bits;
        byte full;
    };
};

class ModRM {
public:
    static ModRM decode(CPUState &state, Memory &memory) {
        ModRMByte modrm_byte(imp::fetch_operand<byte>(state, memory));
        word disp;
        // If we're dealing with register to register, there's no address
        // computation required, so we can leave early.
        if (modrm_byte.bits.mode == 0b11) {
            return ModRM(modrm_byte, 0);
        }

        if (modrm_byte.bits.mode == 0) {
            if (modrm_byte.bits.rm == 0b110) {
                // When loading an from an absolute address, we
                // read at the memory specified directly in the operand
                // and that's it.
                state.cpu_time += 6;
                address addr = state.data_segment() + imp::fetch_operand<word>(state, memory);
                return ModRM(modrm_byte, addr);
            } else {
                // There is no displacement byte in mode 0 when rm is 110.
                disp = 0;
            }
        } else if (modrm_byte.bits.mode == 0b01) {
            // When displacement is happening, we take a 4 cycle hit.
            // This is an 8 bit displacement however, so we read a single
            // byte for the offset and stretch to a word.
            state.cpu_time += 4;
            disp = sign_extend(imp::fetch_operand<byte>(state, memory));
        } else {
            // When displacement is happening, we take a 4 cycle hit
            state.cpu_time += 4;
            disp = imp::fetch_operand<word>(state, memory);
        }

        address addr;
        switch (modrm_byte.bits.rm) {
            case 0: {
                state.cpu_time += 7;
                addr = state.data_segment() + state.bx() + state.si();
            } break;
            case 1: {
                state.cpu_time += 8;
                addr = state.data_segment() + state.bx() + state.di();
            } break;
            case 2: {
                state.cpu_time += 8;
                addr = state.stack_segment() + state.bp() + state.si();
            } break;
            case 3: {
                state.cpu_time += 7;
                addr = state.stack_segment() + state.bp() + state.di();
            } break;
            case 4: {
                state.cpu_time += 5;
                addr = state.data_segment() + state.si();
            } break;
            case 5: {
                state.cpu_time += 5;
                addr = state.data_segment() + state.di();
            } break;
            case 6: {
                state.cpu_time += 5;
                addr = state.stack_segment() + state.bp();
            } break;
            case 7: {
                state.cpu_time += 5;
                addr = state.data_segment() + state.bx();
            } break;
            default: {
                throw std::runtime_error("Unexpected rm byte");
            }
        }
        return ModRM(modrm_byte, addr + disp);
    }

    ModRM(const ModRMByte &modrm_byte, address addr);

    ModRMByte modrm_byte() const;
    address effective_address() const;

private:
    ModRMByte _byte;
    address _address;
};

inline ModRM::ModRM(const ModRMByte &modrm_byte, address addr) : _byte{modrm_byte}, _address(addr) {
}

inline ModRMByte ModRM::modrm_byte() const {
    return _byte;
}
inline address ModRM::effective_address() const {
    return _address;
}
} // namespace empc
