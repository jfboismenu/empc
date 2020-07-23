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

TEST_CASE("Register tests", "[cpu][registers]")
{
    SECTION("General Purpose Registers")
    {
        SECTION("Data registers can read and write")
        {
            empc::CPUState regs;

            regs.al() = 0x01;
            regs.ah() = 0x02;
            regs.bl() = 0x03;
            regs.bh() = 0x04;
            regs.cl() = 0x06;
            regs.ch() = 0x07;
            regs.dl() = 0x08;
            regs.dh() = 0x09;

            REQUIRE(regs.al() == 0x01);
            REQUIRE(regs.ah() == 0x02);
            REQUIRE(regs.bl() == 0x03);
            REQUIRE(regs.bh() == 0x04);
            REQUIRE(regs.cl() == 0x06);
            REQUIRE(regs.ch() == 0x07);
            REQUIRE(regs.dl() == 0x08);
            REQUIRE(regs.dh() == 0x09);

            regs.ax() = 0xA0B0;
            regs.bx() = 0xC0D0;
            regs.cx() = 0xE0F0;
            regs.dx() = 0x0010;

            REQUIRE(regs.ax() == 0xA0B0);
            REQUIRE(regs.bx() == 0xC0D0);
            REQUIRE(regs.cx() == 0xE0F0);
            REQUIRE(regs.dx() == 0x0010);
        }

        SECTION("Pointer and index registers can read and write")
        {
            empc::CPUState regs;

            regs.si() = 0x12;
            regs.di() = 0x34;
            regs.bp() = 0x56;
            regs.sp() = 0x78;

            REQUIRE(regs.si() == 0x12);
            REQUIRE(regs.di() == 0x34);
            REQUIRE(regs.bp() == 0x56);
            REQUIRE(regs.sp() == 0x78);
        }
    }

    SECTION("Segment registers")
    {
        empc::CPUState regs;

        regs.cs() = 0x12;
        regs.ds() = 0x34;
        regs.es() = 0x56;
        regs.ss() = 0x78;

        REQUIRE(regs.cs() == 0x12);
        REQUIRE(regs.ds() == 0x34);
        REQUIRE(regs.es() == 0x56);
        REQUIRE(regs.ss() == 0x78);
    }
}
