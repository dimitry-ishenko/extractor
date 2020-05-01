#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <vector>

using byte = unsigned char;
using payload = std::vector<byte>;

using timestamp = std::uint64_t;
constexpr timestamp no_time = -1;

#endif
