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

#include <empc/empc.h>
#include <empc/memory/memory.imp.h>
#include <istream>

namespace empc {

EmPC::EmPC() : _memory{1024 * 1024}, _cpu{_memory} {
}

const CPU &EmPC::cpu() const {
    return _cpu;
}

void EmPC::load_bios(std::istream &stream) {
    _memory.write_region(0xF0000, stream);
}

void EmPC::reset() {
    _cpu.reset();
}

void EmPC::emulate() {
    while (true) {
        emulate_once();
    }
}

void EmPC::emulate_once() {
    _cpu.emulate_once();
}

} // namespace empc
