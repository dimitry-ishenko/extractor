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
                case  1: if(fld.type == protobuf::embed ) word.text = fld.data.to_string(); break;
                case  2: if(fld.type == protobuf::embed ) word.text2 = fld.data.to_string(); break;
                case  3: if(fld.type == protobuf::varint) word.start = ms(fld.data.value); break;
                case  4: if(fld.type == protobuf::varint) word.end = ms(fld.data.value); break;
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

void script::save_to(const std::string& name)
try
{
    std::cout << "Opening transcript file: " << name << std::endl;
    std::fstream fs;
    fs.exceptions(std::ios::failbit | std::ios::badbit);
    fs.open(name, std::ios::out | std::ios::trunc);

    auto paras = 0, words = 0, dropped = 0;

    std::cout << "Saving transcript" << std::endl;
    for(auto const& wd : words_)
    {
        auto text = wd.get_text();
        if(text.size())
        {
            if(text[0] == '\n')
            {
                if(wd.start.count()) fs << "\n\n[" << wd.get_time() << "]\n";

                text.erase(0, 1);
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
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
}

std::string script::word::get_time() const
{
    using namespace std::chrono;
    auto hrs = duration_cast<hours>(start).count();
    auto min = duration_cast<minutes>(start).count() % 60;
    auto sec = duration_cast<seconds>(start).count() % 60;

    auto as_text = [](int n) { return (n > 9 ? "" : "0") + std::to_string(n); };

    return (hrs ? as_text(hrs) + ":" : "") + as_text(min) + ":" + as_text(sec);
}

}
