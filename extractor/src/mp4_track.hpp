#ifndef MP4_TRACK_HPP
#define MP4_TRACK_HPP

#include "mp4_sample.hpp"
#include "Ap4.h"

namespace mp4
{

class track
{
public:
    track() = default;
    bool is_valid() const { return track_; }

    mp4::sample read_sample();

private:
    AP4_Track* track_ = nullptr;
    AP4_SampleTable* table_ = nullptr;
    std::size_t sample_ = 0, count_ = 0;

    friend class file;
    explicit track(AP4_Track*);
};

}

#endif