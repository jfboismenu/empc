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

#include <catch2/catch.hpp>
#include <empc/cpu/cpu.h>
#include <empc/cpu/modrm.h>
#include <empc/memory/memory.imp.h>

void setModRM(empc::Memory &mem, empc::byte mode, empc::byte rm) {
    empc::ModRMByte data(0);
    data.bits.mode = mode;
    data.bits.rm = rm;
    mem.write(0xFFFF1, data.full);
}

SCENARIO("ModRM byte tests", "[cpu][modrm]") {
    empc::Memory memory(1024 * 1024);
    empc::CPUState state;
    state.reset();
    state.bx() = 0x100;
    state.bp() = 0x1000;
    state.sp() = 0x2000;
    state.si() = 0x4000;
    state.di() = 0x8000;
    state.ss() = 0xE000;
    state.ds() = 0xD000;

    // Assuming the displacement for all operations will be 0x80
    // Here we'll write the expected value for each memory addressing possible
    // in ModRM
    // DS + DISP
    memory.write(0xD0080, 0x0102);
    // DS + BX + SI + DISP
    memory.write(0xD4180, 0x0304);
    // DS + BX + DI + DISP
    memory.write(0xD8180, 0x0506);
    // SS + BP + SI + DISP
    memory.write(0xE3080, 0x0708);
    // SS + BP + DI + DISP
    memory.write(0xE9080, 0x090A);
    // DS + SI + DISP
    memory.write(0xD4080, 0x0B0C);
    // DS + DI + DISP
    memory.write(0xD8080, 0x0D0E);
    // SS + BP + DISP
    memory.write(0xE1080, 0x0F10);
    // DS + BX + DISP
    memory.write(0xD0180, 0x1112);

    // MOV reg16 <- mem16
    memory.write(0xFFFF0, 0x8B);
    memory.write(0xFFFF2, 0x80);
    memory.write(0xFFFF3, 0x00);

    WHEN("Mod is 10 and rm is 000") {
        setModRM(memory, 0b10, 0);
        state.ip() += 1;
        empc::ModRM modrm(empc::ModRM::decode(state, memory));
        REQUIRE(modrm.effective_address() == 0xD4180);
    }

    WHEN("Mod is 01") {
    }

    WHEN("Mod is 10") {
    }
}
