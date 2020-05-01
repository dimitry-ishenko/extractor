#include "protobuf.hpp"
#include "trans.hpp"

#include <exception>
#include <iostream>
#include <iterator>

namespace trans
{

piece decode(const payload& data)
{
    trans::piece piece;
    auto pos = data.begin(), end = data.end();

    std::advance(pos, 4); // skip 4 bytes - dunno what they are

    while(pos < end)
    try
    {
        auto msg = protobuf::parse_field(pos, end);
        if(msg.id == 1 && msg.type == protobuf::embed)
        {
            trans::word word;

            while(msg.data.from < msg.data.to)
            {
                auto fld = protobuf::parse_field(msg.data.from, msg.data.to);
                switch(fld.id)
                {
                case 1:
                    if(fld.type == protobuf::embed)
                        word.text = fld.data.as_string();
                    break;

                case 2:
                    if(fld.type == protobuf::embed)
                        word.text2 = fld.data.as_string();
                    break;

                case 3:
                    if(fld.type == protobuf::varint)
                        word.start = fld.data.value;
                    break;

                case 4:
                    if(fld.type == protobuf::varint)
                        word.end = fld.data.value;
                    break;
                }
            }

            piece.words.push_back(std::move(word));
        }
        else if(msg.id == 2 && msg.type == protobuf::fixed32)
        {
            piece.len = *(float*)(&msg.data.value);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return piece;
}

}
