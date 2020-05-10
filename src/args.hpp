#ifndef ARGS_HPP
#define ARGS_HPP

#include <filesystem>
#include <string>

std::string usage(const std::string& name);

struct args
{
    bool help = false;
    int rate = 0;
    std::filesystem::path script;
    bool version = false;
    std::filesystem::path audio;
};

args read_args(int argc, char* argv[]);

#endif
