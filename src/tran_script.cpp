#include "protobuf.hpp"
#include "tran_script.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>

namespace tran
{

void script::add_words(const payload& data)
{
    auto count = 0;
    auto msg_dropped = 0, fld_dropped = 0;

    auto pos = data.begin(), end = data.end();
    std::advance(pos, 4);

    while(pos < end)
    try
    {
        auto msg = protobuf::parse_field(pos, end);
        if(msg.id == 1 && msg.type == protobuf::embed)
        {
            script::word word;

            while(msg.data.from < msg.data.to)
            {
                auto fld = protobuf::parse_field(msg.data.from, msg.data.to);
                switch(fld.id)
                {
                case  1: if(fld.type == protobuf::embed ) word.text  = fld.data.to_string( ); break;
                case  2: if(fld.type == protobuf::embed ) word.text2 = fld.data.to_string( ); break;
                case  3: if(fld.type == protobuf::varint) word.start = ms { fld.data.value }; break;
                case  4: if(fld.type == protobuf::varint) word.end   = ms { fld.data.value }; break;
                default: fld_dropped++;
                }
            }

            words_.push_back(std::move(word));
            count++;
        }
        else msg_dropped++;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Decoded: " << count << " words" << std::endl;
    std::cout << "Dropped: " << msg_dropped << " messages, " << fld_dropped << " fields" << std::endl;
}

void script::save_to(const std::filesystem::path& path, int rate)
{
    auto paras = 0, words = 0, dropped = 0;

    std::cout << "Opening transcript file: " << path << std::endl;
    std::fstream fs;
    fs.exceptions(std::ios::failbit | std::ios::badbit);
    fs.open(path, std::ios::out | std::ios::trunc);

    if(rate) std::cout << "Adding time-stamps every " << (
        rate == 1 ? "" :
        rate == 2 ? "2nd " :
        rate == 3 ? "3rd " : std::to_string(rate) + "th "
    ) << "paragraph" << std::endl;

    for(auto const& word : words_)
    {
        if(auto text = word.get_text(); text.size())
        {
            if(text[0] == '\n')
            {
                text.erase(0, 1);
                if(paras) fs << std::endl << std::endl;

                if(rate && paras % rate == 0)
                {
                    if(word.start > ms(0)) fs << "[" + word.get_time() + "]" << std::endl;
                }
                paras++;
            }

            fs << text << ' ';
            words++;
        }
        else dropped++;
    }
    fs << std::endl;

    fs.close();

    std::cout << "Saved: " << words << " words in " << paras << " paragraphs" << std::endl;
    std::cout << "Dropped: " << dropped << " words" << std::endl;
}

std::string script::word::get_time() const
{
    using namespace std::chrono;
    auto h = duration_cast<hours>(start).count();
    auto m = duration_cast<minutes>(start).count() % 60;
    auto s = duration_cast<seconds>(start).count() % 60;

    auto as_text = [](int n) { return (n > 9 ? "" : "0") + std::to_string(n); };

    return (h ? as_text(h) + ":" : "") + as_text(m) + ":" + as_text(s);
}

}
