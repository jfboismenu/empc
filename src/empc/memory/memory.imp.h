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

#include <empc/memory/memory.h>
#include <iostream>
#include <memory>

namespace empc {

inline Memory::Memory(size_t size)
    : _bytes(size, byte { 0 })
{
}

inline byte Memory::read_byte(address addr) const
{
    return _bytes[addr];
}

inline void Memory::write_byte(address addr, byte data)
{
    _bytes[addr] = data;
}

inline word Memory::read_word(address addr) const
{
    return reinterpret_cast<const word&>(_bytes[addr]);
}

inline void Memory::write_word(address addr, word data)
{
    reinterpret_cast<word&>(_bytes[addr]) = data;
}

inline void Memory::write_region(address addr, std::istream& stream)
{
    char* current = reinterpret_cast<char*>(&_bytes[addr]);

    // get length of file:
    stream.seekg(0, stream.end);
    const auto length { stream.tellg() };
    stream.seekg(0, stream.beg);

    stream.read(current, length);
}

}
