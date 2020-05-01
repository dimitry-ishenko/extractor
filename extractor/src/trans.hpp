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

using words = std::vector<word>;

words decode(const payload&);

}

#endif
