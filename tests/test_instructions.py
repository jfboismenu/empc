# MIT License
#
# Copyright (c) 2020 Jean-François Boismenu
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from ctypes import CDLL, c_short, pointer, POINTER, c_int, c_char_p
import pytest
from collections import namedtuple


@pytest.fixture(scope="session")
def empc():
    empc = CDLL("libtest-instructions.so")
    empc.reset.restype = None

    empc.write_memory.restype = None
    empc.write_memory.argtypes = (c_int, c_char_p, c_int)

    empc.emulate_once.restype = None
    empc.emulate_once.argtypes = ()

    dword_p = POINTER(c_int)
    word_p = POINTER(c_short)

    registers_args = [
        (dword_p, "eax"),
        (dword_p, "ebx"),
        (dword_p, "ecx"),
        (dword_p, "edx"),
        (dword_p, "esi"),
        (dword_p, "edi"),
        (dword_p, "ebp"),
        (dword_p, "esp"),
        (dword_p, "pc"),
        (word_p, "cs"),
        (word_p, "ds"),
        (word_p, "es"),
        (word_p, "fs"),
        (word_p, "gs"),
        (word_p, "ss"),
    ]

    empc.read_registers.restype = None
    empc.read_registers.argtypes = tuple(argtype for argtype, _ in registers_args)

    empc.write_registers.restype = None
    empc.write_registers.argtypes = tuple(argtype for argtype, _ in registers_args)

    Registers = namedtuple(
        "Registers", tuple(register_name for _, register_name in registers_args)
    )

    class EmPC:
        reset = empc.reset
        write_memory = empc.write_memory
        emulate_once = empc.emulate_once

        @staticmethod
        def read_registers():
            # Allocate the right data type for each register type.
            args = [argtype._type_() for argtype, _ in registers_args]
            pargs = [pointer(arg) for arg in args]
            print(pargs)
            # pass a pointer for each argument to the function.
            empc.read_registers(*pargs)
            # Turn each of the resulting values into ints.
            return Registers(*[arg.value for arg in args])

        @staticmethod
        def write_registers(**args):
            args = [
                (argtype._type_(args[register_name]) if register_name in args else None)
                for argtype, register_name in registers_args
            ]
            pargs = [(pointer(arg) if arg else arg) for arg in args]
            print(pargs)
            empc.write_registers(*pargs)
            return Registers(*[arg.value for arg in args])

    return EmPC


def test_read_write_registers(empc):

    assert empc.write_registers(
        eax=1,
        ebx=2,
        ecx=3,
        edx=4,
        esi=5,
        edi=6,
        ebp=7,
        esp=8,
        pc=9,
        cs=10,
        ds=11,
        es=12,
        fs=13,
        gs=14,
        ss=15,
    )
    registers = empc.read_registers()

    assert registers.eax == 1
    assert registers.ebx == 2
    assert registers.ecx == 3
    assert registers.edx == 4
    assert registers.esi == 5
    assert registers.edi == 6
    assert registers.ebp == 7
    assert registers.esp == 8
    assert registers.pc == 9
    assert registers.cs == 10
    assert registers.ds == 11
    assert registers.es == 12
    assert registers.fs == 13
    assert registers.gs == 14
    assert registers.ss == 15
