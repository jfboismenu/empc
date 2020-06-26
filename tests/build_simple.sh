#!/usr/bin/env bash
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

set -e

OUTPUT_DIR=$1
TESTS_ROOT=$2
CPU_TARGET=$3

if [ ${CPU_TARGET} == x64 ]; then
    g++ $TESTS_ROOT/test_simple.cpp -o $OUTPUT_DIR/libtestsimple.x64.so -O0 -shared -fPIC
else
    g++ -o $OUTPUT_DIR/test_simple.o         -O0 -c $TESTS_ROOT/test_simple.cpp
    ld  -o $OUTPUT_DIR/libtestsimple.8086.so -shared $OUTPUT_DIR/test_simple.o
    objdump -Mi8086 -S $OUTPUT_DIR/libtestsimple.8086.so
fi
