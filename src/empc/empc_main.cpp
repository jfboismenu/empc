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

#include <empc/empc.h>
#include <empc/debug.h>

#include <docopt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <iostream>

const char USAGE[] =
    R"(EMpathy PC emulator

Usage:
    empc test <path-to-binary> [--nb-iterations=<nb-iterations>]

Options:
    --nb-iterations nb-iterations  Number of times to run the test.  [default: 1000000]

)";

int main(int argc, char** argv)
{
    auto console = spdlog::stdout_color_mt("console");
    auto args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,
        "EMpathy PC emulator 0.1");

    if (args["test"].asBool()) {
        const std::string path_to_binary(args["<path-to-binary>"].asString());
        if (!std::filesystem::exists(path_to_binary)) {
            spdlog::get("console")->info("Binary not found on disk: {}", path_to_binary);
            return -1;
        }

        console->info("Starting emulation");

        empc::EmPC pc;
        std::ifstream ifs(path_to_binary);
        pc.load_bios(ifs);

        unsigned int nb_instructions{0};
        const long nb_iterations(args["--nb-iterations"].asLong());
        for(unsigned i = 0; i < nb_iterations; ++i) {
            pc.reset();
            while(!pc.cpu().is_halted()) {
                pc.emulate_once();
                ++nb_instructions;
            }
        }
        console->info("Nb instructions {}", nb_instructions);
    }
    return 0;
}
