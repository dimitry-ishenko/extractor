#ifndef PROTOBUF_HPP
#define PROTOBUF_HPP

#include "types.hpp"

#include <cstdint>
#include <string>

namespace protobuf
{

enum type
{
    varint  = 0,
    fixed64 = 1,
    embed   = 2,
    fixed32 = 5,
};

using iterator = payload::const_iterator;
using value = std::uint64_t;

struct data
{
    iterator from, to;
    protobuf::value value;

    auto to_string() const { return std::string(from, to); }
};

struct field
{
    int id;
    protobuf::type type;
    protobuf::data data;
};

field parse_field(iterator& pos, const iterator& end);

}

#endif
