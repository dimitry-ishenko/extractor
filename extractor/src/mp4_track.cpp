#include "mp4_track.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

namespace mp4
{

track::track(AP4_Track* trk) : track_(trk)
{
    std::cout << "Processing track: ID=" << track_->GetId() << " name=" << track_->GetTrackName() << std::endl;
    table_ = track_->GetSampleTable();
    count_ = table_->GetSampleCount();
    std::cout << "Found " << count_ << " samples" << std::endl;
}

payload track::read_sample()
{
    payload data;

    if(sample_ < count_)
    try
    {
        std::cout << "Reading sample: " << sample_ << std::endl;

        AP4_Sample sample;
        auto res = table_->GetSample(sample_, sample);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to read sample");

        std::unique_ptr<AP4_DataBuffer> temp { new AP4_DataBuffer() };
        res = sample.ReadData(*temp);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to read sample data");

        sample_++;
        data = payload { temp->GetData(), temp->GetData() + temp->GetDataSize() };

        std::cout << "Read " << data.size() << " bytes" << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return data;
}

}
