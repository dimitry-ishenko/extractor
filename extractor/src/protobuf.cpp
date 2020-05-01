#include "protobuf.hpp"

#include <iterator>
#include <stdexcept>
#include <string>

namespace protobuf
{

value parse_varint(iterator& pos, const iterator& end)
{
    value val = 0;
    for(auto i = 0; ; ++i)
    {
        if(i >= 8) throw std::invalid_argument("Overflow while parsing varint");
        if(pos == end) throw std::invalid_argument("EOS while parsing varint");

        auto ch = *(pos++);
        val |= (ch & 0x7f) << (i * 7);

        if((ch & 0x80) == 0) break;
    }
    return val;
}

value parse_fixed(int n, iterator& pos, const iterator& end)
{
    value val = 0;
    for(auto i = 0; i < n; ++i)
    {
        if(pos == end)
        {
            auto name = std::to_string(n * 8);
            throw std::invalid_argument("EOS while parsing fixed" + name);
        }
        val |= *(pos++) << (i * 8);
    }
    return val;
}

iterator parse_embed(iterator& pos, const iterator& end)
{
    auto size = parse_varint(pos, end);
    auto from = pos;

    std::advance(pos, size);
    if(pos > end) throw std::invalid_argument("EOS while parsing data");

    return from;
}

field parse_field(iterator& pos, const iterator& end)
{
    auto val = parse_varint(pos, end);

    field fld;
    fld.id = val >> 3;
    fld.type = static_cast<type>(val & 0x07);

    switch(fld.type)
    {
    case varint:
        fld.data.from = pos;
        fld.data.value = parse_varint(pos, end);
        fld.data.to = pos;
        break;

    case fixed64:
        fld.data.from = pos;
        fld.data.value = parse_fixed(8, pos, end);
        fld.data.to = pos;
        break;

    case embed:
        fld.data.from = parse_embed(pos, end);
        fld.data.to = pos;
        break;

    case fixed32:
        fld.data.from = pos;
        fld.data.value = parse_fixed(4, pos, end);
        fld.data.to = pos;
        break;

    default: throw std::invalid_argument("Invalid wire type");
    }

    return fld;
}

}
