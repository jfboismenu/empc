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

enum class Register {
    AX,
    BX,
    CX,
    DX,
    SI,
    DI,
    BP,
    SP,
    CS,
    DS,
    ES,
    SS,
    IP,
    AH,
    AL,
    BH,
    BL,
    CH,
    CL,
    DH,
    DL
};

template <typename DATA_TYPE> struct RegisterTypeBase { using DataType = DATA_TYPE; };

template <Register reg> struct RegisterType;

template <> struct RegisterType<Register::AX> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::AL> : public RegisterTypeBase<byte> {};
template <> struct RegisterType<Register::AH> : public RegisterTypeBase<byte> {};

template <> struct RegisterType<Register::BX> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::BL> : public RegisterTypeBase<byte> {};
template <> struct RegisterType<Register::BH> : public RegisterTypeBase<byte> {};

template <> struct RegisterType<Register::CX> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::CL> : public RegisterTypeBase<byte> {};
template <> struct RegisterType<Register::CH> : public RegisterTypeBase<byte> {};

template <> struct RegisterType<Register::DX> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::DL> : public RegisterTypeBase<byte> {};
template <> struct RegisterType<Register::DH> : public RegisterTypeBase<byte> {};

template <> struct RegisterType<Register::SP> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::BP> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::SI> : public RegisterTypeBase<word> {};
template <> struct RegisterType<Register::DI> : public RegisterTypeBase<word> {};

} // namespace empc
