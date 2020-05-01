#include "protobuf.hpp"
#include "trans.hpp"

#include <exception>
#include <iostream>
#include <iterator>

namespace trans
{

words decode(const payload& data)
{
    words wds;

    auto pos = data.begin(), end = data.end();
    std::advance(pos, 4);

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
                }
            }

            wds.push_back(std::move(wd));
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return wds;
}

}
