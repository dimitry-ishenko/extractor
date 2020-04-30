#ifndef MP4_SAMPLE_HPP
#define MP4_SAMPLE_HPP

#include "Ap4.h"

#include <memory>
#include <utility>

namespace mp4
{

class sample
{
public:
    sample() = default;
    bool is_valid() const { return !!data_; }

    auto data() const { return data_ ? data_->GetData() : nullptr; }
    auto size() const { return data_ ? data_->GetDataSize() : 0; }

private:
    std::unique_ptr<AP4_DataBuffer> data_;

    friend class track;
    explicit sample(std::unique_ptr<AP4_DataBuffer> data) :
        data_(std::move(data))
    { }
};

}

#endif
