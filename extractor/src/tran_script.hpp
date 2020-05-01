#ifndef TRAN_SCRIPT_HPP
#define TRAN_SCRIPT_HPP

#include "types.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace tran
{

class script
{
public:
    script() = default;

    void add_words(const payload&);
    void save_to(const std::string&);

private:
    using ms = std::chrono::milliseconds;

    struct word
    {
        std::string text, text2;
        ms start, end;

        auto const& get_text() const { return text2.size() ? text2 : text; }
        std::string get_time() const;
    };
    using words = std::vector<word>;

    words words_;
};

}

#endif
