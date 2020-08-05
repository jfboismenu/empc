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
#include <empc/memory/memory.imp.h>

TEST_CASE("Memory", "[memory]") {
    empc::Memory buffer{16};

    buffer.write_byte(0, 1);
    REQUIRE(buffer.read<byte>(0) == 1);
    REQUIRE(buffer.read<word>(0) == 1);

    buffer.write_byte(1, 2);
    REQUIRE(buffer.read<byte>(0) == 1);
    REQUIRE(buffer.read<byte>(1) == 2);
    REQUIRE(buffer.read<word>(0) == 513);
    REQUIRE(buffer.read<word>(1) == 2);

    buffer.write_word(0, 0);
    REQUIRE(buffer.read<word>(0) == 0);
    REQUIRE(buffer.read<byte>(0) == 0);
    REQUIRE(buffer.read<word>(1) == 0);

    buffer.write_word(0, 1026);
    REQUIRE(buffer.read<byte>(0) == 2);
    REQUIRE(buffer.read<byte>(1) == 4);
}
