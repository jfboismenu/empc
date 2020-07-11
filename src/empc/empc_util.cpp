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
#include <ostream>
#include <iomanip>

namespace {
template<typename T>
void write_register(std::ostream& os, T value, const char* name, const bool last=false) {
    os << name << ": 0x" << std::hex << std::setw(4) << std::setfill('0') << value;
    if (!last) {
        os << " ";
    }
}
}

namespace empc
{

std::ostream &get_state(std::ostream& os, const CPU &cpu)
{
    write_register(os, cpu.data_registers().ax(), "ax");
    write_register(os, cpu.data_registers().bx(), "bx");
    write_register(os, cpu.data_registers().cx(), "cx");
    write_register(os, cpu.data_registers().dx(), "dx");
    write_register(os, cpu.pointer_and_index_registers().si(), "si");
    write_register(os, cpu.pointer_and_index_registers().di(), "di");
    write_register(os, cpu.pointer_and_index_registers().bp(), "bp");
    write_register(os, cpu.pointer_and_index_registers().sp(), "sp");
    write_register(os, cpu.instruction_pointer_register().ip(), "ip");
    write_register(os, cpu.segment_registers().cs(), "cs");
    write_register(os, cpu.segment_registers().ds(), "ds");
    write_register(os, cpu.segment_registers().es(), "es");
    write_register(os, cpu.segment_registers().ss(), "ss", true);
    return os;
}

}
