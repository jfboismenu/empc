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
#include <fmt/core.h>

empc::ModRM prep(empc::CPUState &state, empc::Memory &memory, empc::byte mode, empc::byte rm) {
    empc::ModRMByte data(0);
    data.bits.mode = mode;
    data.bits.rm = rm;
    memory.write(0xFFFF1, data.full);
    state.ip() += 1;
    return empc::ModRM::decode(state, memory);
}

empc::Memory memory(1024 * 1024);
empc::CPUState state;

TEST_CASE("ModRM byte tests", "[cpu][modrm]") {
    SECTION("Testing effective addresses and cycle counts") {
        auto [mod, rm, effective_address, nb_cycles] = GENERATE(
            (std::make_tuple(0b10, 0b000, 0xD4181, 11)),
            (std::make_tuple(0b10, 0b001, 0xD8181, 12)),
            (std::make_tuple(0b10, 0b010, 0xE5180, 12)),
            (std::make_tuple(0b10, 0b011, 0xE9180, 11)), (std::make_tuple(0b10, 0b100, 0xD4180, 9)),
            (std::make_tuple(0b10, 0b101, 0xD8180, 9)), (std::make_tuple(0b10, 0b110, 0xE1180, 9)),
            (std::make_tuple(0b10, 0b111, 0xD0181, 9)),

            (std::make_tuple(0b00, 0b000, 0xD4001, 7)), (std::make_tuple(0b00, 0b001, 0xD8001, 8)),
            (std::make_tuple(0b00, 0b010, 0xE5000, 8)), (std::make_tuple(0b00, 0b011, 0xE9000, 7)),
            (std::make_tuple(0b00, 0b100, 0xD4000, 5)), (std::make_tuple(0b00, 0b101, 0xD8000, 5)),
            (std::make_tuple(0b00, 0b110, 0xD0180, 6)), (std::make_tuple(0b00, 0b111, 0xD0001, 5)),

            (std::make_tuple(0b01, 0b000, 0xE3F81, 11)),
            (std::make_tuple(0b01, 0b001, 0xE7F81, 12)),
            (std::make_tuple(0b01, 0b010, 0xF4F80, 12)),
            (std::make_tuple(0b01, 0b011, 0xF8F80, 11)), (std::make_tuple(0b01, 0b100, 0xE3F80, 9)),
            (std::make_tuple(0b01, 0b101, 0xE7F80, 9)), (std::make_tuple(0b01, 0b110, 0xF0F80, 9)),
            (std::make_tuple(0b01, 0b111, 0xDFF81, 9)),

            (std::make_tuple(0b11, 0b000, 0, 0)), (std::make_tuple(0b11, 0b001, 0, 0)),
            (std::make_tuple(0b11, 0b010, 0, 0)), (std::make_tuple(0b11, 0b011, 0, 0)),
            (std::make_tuple(0b11, 0b100, 0, 0)), (std::make_tuple(0b11, 0b101, 0, 0)),
            (std::make_tuple(0b11, 0b110, 0, 0)), (std::make_tuple(0b11, 0b111, 0, 0)));

        state.reset();
        state.bx() = 0x0001;
        state.bp() = 0x1000;
        state.sp() = 0x2000;
        state.si() = 0x4000;
        state.di() = 0x8000;
        state.ss() = 0xE000;
        state.ds() = 0xD000;

        // MOV reg16 <- mem16
        memory.write(0xFFFF0, 0x8B);
        memory.write(0xFFFF2, 0x80);
        memory.write(0xFFFF3, 0x01);

        WHEN(fmt::format("mod is {:02b} and rm is {:03b}", mod, rm)) {
            THEN(fmt::format("{} cycles are spent computing effective address {:05x}", nb_cycles,
                             effective_address)) {
                auto modrm = prep(state, memory, mod, rm);
                REQUIRE(modrm.effective_address() == effective_address);
                REQUIRE(state.cpu_time == nb_cycles);
            }
        }
    }
}
