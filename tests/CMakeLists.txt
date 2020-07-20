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

include_directories(empc-tests INTERFACE ".")
include_directories(empc-tests INTERFACE "../src")

# Grab all CPP files to run C++ runs
FILE(GLOB CppTestsSources */*.cpp)
add_executable(empc_tests empc_tests.cpp ${CppTestsSources})
target_link_libraries(empc_tests CONAN_PKG::Catch2)
target_link_libraries(empc_tests empclib)

add_test(empc-tests empc_tests)

# Compile all assembly files.
add_custom_command(OUTPUT test_mov.bin MAIN_DEPENDENCY test_mov.asm COMMAND nasm -f bin "${CMAKE_CURRENT_SOURCE_DIR}/test_mov.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/test_mov.bin")
# Add them to a custom target for assemblies.
add_custom_target(assemblies ALL DEPENDS test_mov.bin)
