
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

#include <empc/base/types.h>
#include <empc/cpu/registers_common.h>

namespace empc {

class SegmentRegisters {
public:
    SegmentRegisters();

    const word& cs() const;
    word& cs();
    const word& ds() const;
    word& ds();
    const word& es() const;
    word& es();
    const word& ss() const;
    word& ss();

private:
    word _cs;
    word _ds;
    word _es;
    word _ss;
};

inline SegmentRegisters::SegmentRegisters()
    : _cs { 0 }
    , _ds { 0 }
    , _es { 0 }
    , _ss { 0 }
{
}

inline const word& SegmentRegisters::cs() const
{
    return _cs;
}
inline word& SegmentRegisters::cs()
{
    return _cs;
}
inline const word& SegmentRegisters::ds() const
{
    return _ds;
}
inline word& SegmentRegisters::ds()
{
    return _ds;
}
inline const word& SegmentRegisters::es() const
{
    return _es;
}
inline word& SegmentRegisters::es()
{
    return _es;
}
inline const word& SegmentRegisters::ss() const
{
    return _ss;
}
inline word& SegmentRegisters::ss()
{
    return _ss;
}
}
