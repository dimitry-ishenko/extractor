#ifndef TRANS_HPP
#define TRANS_HPP

#include "types.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace trans
{

using ms = std::chrono::milliseconds;
using timestamp = ms;

struct word
{
    std::string text, text2;
    timestamp start, end;

    auto const& get_text() const { return text2.size() ? text2 : text; }
};

using words = std::vector<word>;

class script
{
public:
    script() = default;

    void add_words(const payload&);
    void save_to(const std::string&);

private:
    words words_;
};

}

#endif
