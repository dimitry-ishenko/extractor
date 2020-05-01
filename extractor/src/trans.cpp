#include "protobuf.hpp"
#include "trans.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>

namespace trans
{

words decode(const payload& data)
{
    words wds;

    auto pos = data.begin(), end = data.end();
    std::advance(pos, 4);

    auto msg_dropped = 0, fld_dropped = 0;

    while(pos < end)
    try
    {
        auto msg = protobuf::parse_field(pos, end);
        if(msg.id == 1 && msg.type == protobuf::embed)
        {
            word wd;

            while(msg.data.from < msg.data.to)
            {
                auto fld = protobuf::parse_field(msg.data.from, msg.data.to);
                switch(fld.id)
                {
                case 1:
                    if(fld.type == protobuf::embed)
                        wd.text = fld.data.to_string();
                    break;

                case 2:
                    if(fld.type == protobuf::embed)
                        wd.text2 = fld.data.to_string();
                    break;

                case 3:
                    if(fld.type == protobuf::varint)
                        wd.start = fld.data.value;
                    break;

                case 4:
                    if(fld.type == protobuf::varint)
                        wd.end = fld.data.value;
                    break;

                default: ++fld_dropped;
                }
            }

            wds.push_back(std::move(wd));
        }
        else ++msg_dropped;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Decoded: " << wds.size() << " words" << std::endl;
    std::cout << "Dropped: " << msg_dropped << " messages, " << fld_dropped << " fields" << std::endl;

    return wds;
}

void script::add_words(words wds)
{
    for(auto& wd : wds) words_.push_back(std::move(wd));
}

void script::save_to(const std::string& name)
try
{
    std::fstream fs;
    fs.exceptions(std::ios::failbit | std::ios::badbit);

    fs.open(name, std::ios::out | std::ios::trunc);
    for(auto const& wd : words_) fs << wd.get_text() << ' ';
    fs << std::endl;

    fs.close();
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
}

}
