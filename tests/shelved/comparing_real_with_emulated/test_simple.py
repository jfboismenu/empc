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

FunctionSpecification = namedtuple("FunctionSpecification", ["name", "args", "res"])


@pytest.fixture(scope="session")
def func_specs():
    return [FunctionSpecification("test_movshort", (POINTER(c_short),), None)]


@pytest.fixture(scope="session")
def lib64(func_specs):
    dl = CDLL("libtestsimple.x64.so")
    for spec in func_specs:
        func = getattr(dl, spec.name)
        func.argtypes = spec.args
        func.restype = spec.res
    return dl


@pytest.fixture(scope="session")
def empc():
    empc = CDLL("libempc.so")
    empc.reset.restype = None

    empc.write_memory.restype = None
    empc.write_memory.argtypes = (c_int, c_char_p, c_int)

    empc.push_word.restype = None
    empc.push_word.argtypes = (c_short,)

    empc.read_word.restype = c_short
    empc.read_word.argtypes = (c_int,)

    empc.call.restype = None
    empc.call.argtypes = (c_int,)

    empc.reset()
    with open("libtestsimple.8086.so", "rb") as fh:
        data = fh.read()
        empc.write_memory(0, data, len(data))

    return empc


def _get_func_addr(func_name):
    return 32


def test_movshort(lib64, empc):
    word = c_short(32)
    lib64.test_movshort(pointer(word))
    assert word.value == 3

    # We expect the result to be written at the memory
    # inside the emulator
    empc.push_word(0)
    empc.call(_get_func_addr("test_movshort"))
    assert empc.read_word(0) == 3
