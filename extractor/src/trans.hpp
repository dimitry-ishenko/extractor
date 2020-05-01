#ifndef TRANS_HPP
#define TRANS_HPP

#include "types.hpp"

#include <string>
#include <vector>

namespace trans
{

struct word
{
    std::string text, text2;
    timestamp start = no_time, end = no_time;
};

struct piece
{
    std::vector<word> words;
    float len;
};

piece decode(const payload&);

}

#endif
