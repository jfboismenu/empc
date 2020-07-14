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
#include <fmt/core.h>
#include <ostream>
#include <iomanip>

namespace empc
{

std::string get_state(const CPU &cpu)
{
    return fmt::format(
        "ax: {:04x} "
        "bx: {:04x} "
        "cx: {:04x} "
        "dx: {:04x} "
        "si: {:04x} "
        "di: {:04x} "
        "bp: {:04x} "
        "sp: {:04x} "
        "ip: {:04x} "
        "cs: {:04x} "
        "ds: {:04x} "
        "es: {:04x} "
        "ss: {:04x} ",
        cpu.data_registers().ax(),
        cpu.data_registers().bx(),
        cpu.data_registers().cx(),
        cpu.data_registers().dx(),
        cpu.pointer_and_index_registers().si(),
        cpu.pointer_and_index_registers().di(),
        cpu.pointer_and_index_registers().bp(),
        cpu.pointer_and_index_registers().sp(),
        cpu.instruction_pointer_register().ip(),
        cpu.segment_registers().cs(),
        cpu.segment_registers().ds(),
        cpu.segment_registers().es(),
        cpu.segment_registers().ss()
        );
}

}