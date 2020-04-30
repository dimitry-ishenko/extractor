#include "mp4_sample.hpp"
#include <utility>

mp4::sample::sample(std::unique_ptr<AP4_DataBuffer> data) :
    data_(std::move(data))
{ }
