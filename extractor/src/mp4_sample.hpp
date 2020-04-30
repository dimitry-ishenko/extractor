#ifndef MP4_SAMPLE_HPP
#define MP4_SAMPLE_HPP

#include "Ap4.h"
#include <memory>

namespace mp4
{

class sample
{
public:
    sample() = default;
    bool is_valid() const { return !!data_; }

private:
    std::unique_ptr<AP4_DataBuffer> data_;

    friend class track;
    explicit sample(std::unique_ptr<AP4_DataBuffer>);
};

}

#endif
