# empc

PC emulator with empathy!

# Prerequisites for building.

- Compiler that supports C++20, e.g. `clang++`.
- [`nasm`](https://www.nasm.us/): needed to compile the assembly unit tests.
- [`catch2`](https://github.com/catchorg/Catch2): needed to compile C++ based unit tests.
- [`python`](https://python.org): needed for some Python based unit tests.

`brew install nasm catch2 python3` should cover you if you are on macOS. Follow above links on other platforms.

# Targets

You can only run tests for now. The target is named `test`.
