
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

namespace empc {

class PointerAndIndexRegisters {
public:
    PointerAndIndexRegisters();
    const word& di() const;
    word& di();
    const word& si() const;
    word& si();
    const word& bp() const;
    word& bp();
    const word& sp() const;
    word& sp();

private:
    word _si;
    word _di;
    word _bp;
    word _sp;
};

PointerAndIndexRegisters::PointerAndIndexRegisters()
    : _si { 0 }
    , _di { 0 }
    , _bp { 0 }
    , _sp { 0 }
{
}

inline const word& PointerAndIndexRegisters::si() const
{
    return _si;
}
inline const word& PointerAndIndexRegisters::di() const
{
    return _di;
}
inline const word& PointerAndIndexRegisters::bp() const
{
    return _bp;
}
inline const word& PointerAndIndexRegisters::sp() const
{
    return _sp;
}

inline word& PointerAndIndexRegisters::si()
{
    return _si;
}
inline word& PointerAndIndexRegisters::di()
{
    return _di;
}
inline word& PointerAndIndexRegisters::bp()
{
    return _bp;
}
inline word& PointerAndIndexRegisters::sp()
{
    return _sp;
}

}
