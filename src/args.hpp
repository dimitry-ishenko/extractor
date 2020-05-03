#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>

std::string usage(const std::string& name);

struct args
{
    bool help = false;
    int rate = 0;
    std::string script;
    std::string audio;
};

args read_args(int argc, char* argv[]);

#endif
