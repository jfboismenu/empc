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

#include <docopt.h>
#include <empc/empc.h>
#include <filesystem>
#include <fstream>
#include <iostream>

const char USAGE[] =
    R"(EMpathy PC emulator

Usage:
    empc --bios <path-to-bios>
)";

int main(int argc, char** argv)
{
    auto args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,
        "EMpathy PC emulator 0.1");

    const std::string bios_path(args["<path-to-bios>"].asString());
    if (!std::filesystem::exists(bios_path)) {
        std::cout << "BIOS not found on disk: " << bios_path << std::endl;
        return -1;
    }

    empc::EmPC pc;
    std::ifstream ifs(bios_path.c_str());
    pc.load_bios(ifs);
    pc.reset();
    pc.emulate_once();
    return 0;
}
