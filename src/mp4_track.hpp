#ifndef MP4_TRACK_HPP
#define MP4_TRACK_HPP

#include "types.hpp"
#include "Ap4.h"

namespace mp4
{

class track
{
public:
    track() = default;
    bool is_valid() const { return track_; }

    payload read_sample();

private:
    AP4_Track* track_ = nullptr;
    AP4_SampleTable* table_ = nullptr;
    unsigned sample_ = 0, count_ = 0;

    friend class file;
    explicit track(AP4_Track*);
};

}

#endif
