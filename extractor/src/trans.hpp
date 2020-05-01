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

    auto const& get_text() const { return text2.size() ? text2 : text; }
};

using words = std::vector<word>;

words decode(const payload&);

class script
{
public:
    script() = default;

    void add_words(words);
    void save_to(const std::string&);

private:
    words words_;
};

}

#endif
