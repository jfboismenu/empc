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

    bool is_rm_reg() const;
    bool is_rm_mem() const;
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

inline bool ModRMByte::is_rm_reg() const {
    return bits.mode == 0b11;
}

inline bool ModRMByte::is_rm_mem() const {
    return bits.mode != 0b11;
}

class ModRM {
public:
    static ModRM decode(CPUState &state, Memory &memory);

    ModRM(const ModRMByte &modrm_byte, address addr);

    ModRMByte modrm_byte() const;
    address effective_address() const;

    template <typename T> T read_reg(CPUState &state) const;

    template <typename T> void write_reg(CPUState &state, T value) const;

    template <typename T> T read_rm_mem(Memory &memory) const;

    template <typename T> void write_rm_mem(Memory &memory, T value) const;

    template <typename T> T read_rm_reg(CPUState &state) const;

    template <typename T> void write_rm_reg(CPUState &state, T value) const;

    template <typename T> void write_rm(CPUState &state, Memory &memory, T value) const;

private:
    template <typename T> T &_get_reg(CPUState &state, byte pattern) const;

    const ModRMByte _byte;
    const address _address;
};

inline ModRM ModRM::decode(CPUState &state, Memory &memory) {
    ModRMByte modrm_byte(imp::fetch_operand<byte>(state, memory));
    word disp;
    // If we're dealing with register to register, there's no address
    // computation required, so we can leave early.
    if (modrm_byte.is_rm_reg()) {
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
            // There is no displacement byte in mode 0.
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

inline ModRM::ModRM(const ModRMByte &modrm_byte, address addr) : _byte{modrm_byte}, _address(addr) {
}

inline ModRMByte ModRM::modrm_byte() const {
    return _byte;
}
inline address ModRM::effective_address() const {
    return _address;
}

template <typename T> inline T ModRM::read_reg(CPUState &state) const {
    return _get_reg<T>(state, _byte.bits.reg);
}

template <typename T> inline void ModRM::write_reg(CPUState &state, T value) const {
    _get_reg<T>(state, _byte.bits.reg) = value;
}

template <typename T> inline T ModRM::read_rm_mem(Memory &memory) const {
    return memory.read<T>(_address);
}

template <typename T> inline void ModRM::write_rm_mem(Memory &memory, T value) const {
    memory.write<T>(_address, value);
}

template <typename T> inline T ModRM::read_rm_reg(CPUState &state) const {
    return _get_reg<T>(state, _byte.bits.rm);
}

template <typename T> inline void ModRM::write_rm_reg(CPUState &state, T value) const {
    _get_reg<T>(state, _byte.bits.rm) = value;
}

template <typename T> inline void ModRM::write_rm(CPUState &state, Memory &memory, T value) const {
    if (_byte.is_rm_mem()) {
        write_rm_mem<T>(memory, value);
    } else {
        write_rm_reg<T>(state, value);
    }
}

template <> inline word &ModRM::_get_reg(CPUState &state, byte pattern) const {
    switch (pattern) {
        case 0: {
            return state.ax();
        }
        case 1: {
            return state.cx();
        }
        case 2: {
            return state.dx();
        }
        case 3: {
            return state.bx();
        }
        case 4: {
            return state.sp();
        }
        case 5: {
            return state.bp();
        }
        case 6: {
            return state.si();
        }
        case 7: {
            return state.di();
        }
        default: {
            throw std::runtime_error("Unknown reg");
        }
    }
}

template <> inline byte &ModRM::_get_reg(CPUState &state, byte pattern) const {
    switch (pattern) {
        case 0: {
            return state.al();
        }
        case 1: {
            return state.cl();
        }
        case 2: {
            return state.dl();
        }
        case 3: {
            return state.bl();
        }
        case 4: {
            return state.ah();
        }
        case 5: {
            return state.ch();
        }
        case 6: {
            return state.dh();
        }
        case 7: {
            return state.bh();
        }
        default: {
            throw std::runtime_error("Unknown reg");
        }
    }
}

} // namespace empc
