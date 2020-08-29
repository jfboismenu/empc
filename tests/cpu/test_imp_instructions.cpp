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
#include <empc/cpu/cpu_state.h>
#include <empc/cpu/imp/instruction.h>
#include <fmt/core.h>

using namespace empc;

TEST_CASE("Instruction imp tests", "[cpu][imp]") {
    CPUState state;
    state.ax() = 0x0011;
    state.bx() = 0x2233;
    state.cx() = 0x4455;
    state.dx() = 0x6677;
    state.sp() = 0x8899;
    state.bp() = 0xAABB;
    state.si() = 0xCCDD;
    state.di() = 0xEEFF;

    SECTION("get_reg accesses") {
        WHEN("16-bit register") {
            REQUIRE(imp::get_reg<word>(state, 0) == 0x0011);
            REQUIRE(imp::get_reg<word>(state, 1) == 0x4455);
            REQUIRE(imp::get_reg<word>(state, 2) == 0x6677);
            REQUIRE(imp::get_reg<word>(state, 3) == 0x2233);
            REQUIRE(imp::get_reg<word>(state, 4) == 0x8899);
            REQUIRE(imp::get_reg<word>(state, 5) == 0xAABB);
            REQUIRE(imp::get_reg<word>(state, 6) == 0xCCDD);
            REQUIRE(imp::get_reg<word>(state, 7) == 0xEEFF);
        }
        WHEN("8-bit register") {
            REQUIRE(imp::get_reg<byte>(state, 0) == 0x11);
            REQUIRE(imp::get_reg<byte>(state, 1) == 0x55);
            REQUIRE(imp::get_reg<byte>(state, 2) == 0x77);
            REQUIRE(imp::get_reg<byte>(state, 3) == 0x33);
            REQUIRE(imp::get_reg<byte>(state, 4) == 0x00);
            REQUIRE(imp::get_reg<byte>(state, 5) == 0x44);
            REQUIRE(imp::get_reg<byte>(state, 6) == 0x66);
            REQUIRE(imp::get_reg<byte>(state, 7) == 0x22);
        }
    }
}
