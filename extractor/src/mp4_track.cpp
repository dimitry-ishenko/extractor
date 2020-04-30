#include "mp4_track.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

using std::cout;
using std::endl;

mp4::track::track(AP4_Track* trk) : track_(trk)
{
    cout << "Processing track: ID=" << track_->GetId() << " name=" << track_->GetTrackName() << endl;
    table_ = track_->GetSampleTable();
    count_ = table_->GetSampleCount();
    cout << "Found " << count_ << " samples" << endl;
}

mp4::sample mp4::track::read_sample()
{
    if(sample_ < count_)
    try
    {
        cout << "Reading sample: " << sample_ << endl;

        AP4_Sample sample;
        auto res = table_->GetSample(sample_, sample);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to read sample");

        std::unique_ptr<AP4_DataBuffer> data { new AP4_DataBuffer() };
        res = sample.ReadData(*data);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to read sample data");

        sample_++;
        cout << "Read " << data->GetDataSize() << " bytes" << endl;

        return mp4::sample(std::move(data));
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << endl;
    }

    return mp4::sample();
}
