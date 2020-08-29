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
#include <empc/memory/memory.imp.h>

namespace empc {
namespace imp {
template <typename DataType>
inline DataType fetch_operand(CPUState &state, Memory &memory) noexcept {
    const DataType result{memory.read<DataType>(state.get_program_counter())};
    state.ip() += sizeof(DataType);
    return result;
}

template <typename T> T &get_reg(CPUState &state, byte pattern);

template <> inline word &get_reg<>(CPUState &state, byte pattern) {
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

template <> inline byte &get_reg<>(CPUState &state, byte pattern) {
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

} // namespace imp
} // namespace empc
