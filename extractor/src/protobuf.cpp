#include "protobuf.hpp"

#include <iterator>
#include <stdexcept>
#include <string>

namespace protobuf
{

uint64_t parse_varint(iterator& pos, const iterator& end)
{
    uint64_t value = 0;
    for(auto i = 0; ; ++i)
    {
        if(i >= 8) throw std::invalid_argument("Overflow while parsing varint");
        if(pos == end) throw std::invalid_argument("EOS while parsing varint");

        auto b = *(pos++);
        value |= (b & 0x7f) << (i * 7);

        if((b & 0x80) == 0) break;
    }
    return value;
}

uint64_t parse_fixed(int n, iterator& pos, const iterator& end)
{
    uint64_t value = 0;
    for(auto i = 0; i < n; ++i)
    {
        if(pos == end)
        {
            auto name = std::to_string(n * 8);
            throw std::invalid_argument("EOS while parsing fixed" + name);
        }
        value |= *(pos++) << (i * 8);
    }
    return value;
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
    auto value = parse_varint(pos, end);

    field fld;
    fld.id = value >> 3;
    fld.type = static_cast<type>(value & 0x07);

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
