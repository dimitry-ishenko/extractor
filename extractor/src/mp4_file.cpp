#include "mp4_file.hpp"

#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

constexpr auto meta = AP4_ATOM_TYPE('m', 'e', 't', 'a');
constexpr auto mett = AP4_ATOM_TYPE('m', 'e', 't', 't');

mp4::file::file(const std::string & name)
{
    AP4_ByteStream* s;

    cout << "Opening file: " << name << endl;
    auto res = AP4_FileByteStream::Create(name.data(), AP4_FileByteStream::STREAM_MODE_READ, s);
    if(AP4_FAILED(res)) throw std::invalid_argument("Failed to open file");

    stream_.reset(s);
    file_.reset(new AP4_File { *s });
}

mp4::track mp4::file::find_trans()
{
    auto& tracks = file_->GetMovie()->GetTracks();
    auto count = tracks.ItemCount();
    cout << "Found " << count << " tracks" << endl;

    for(decltype(count) n = 0; n < count; ++n)
    {
        AP4_Track* track;

        cout << "Processing track: " << n << endl;
        auto res = tracks.Get(n, track);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to access track");

        if(track->GetHandlerType() == meta)
        {
            cout << "Detected metadata track: ID=" << track->GetId() << " name=" << track->GetTrackName() << endl;

            auto table = track->GetSampleTable();
            if(!table) throw std::invalid_argument("Missing sample table");

            auto count = table->GetSampleDescriptionCount();
            cout << "Found " << count << " sample descriptions" << endl;

            for(decltype(count) n = 0; n < count; ++n)
            {
                cout << "Processing sample description: " << n << endl;
                auto desc = table->GetSampleDescription(n);

                if(desc->GetFormat() == mett)
                {
                    cout << "Detected timed text format" << endl;
                    if(auto atom = dynamic_cast<AP4_UnknownSampleEntry*>(desc->ToAtom()))
                    {
                        auto payload = atom->GetPayload();
                        std::string type(reinterpret_cast<const char*>(payload.GetData()), payload.GetDataSize());
                        if(type.back() == 0) type.pop_back();

                        // Bento4 eats the first 8 bytes of "application/transcription_1"
                        // thinking they are Reserved and Data reference index fields (6+2)
                        // as per the QuickTime spec

                        if(type == "ion/transcription_1")
                        {
                            cout << "Detected application/transcription_1 MIME type" << endl;
                            return mp4::track(track);
                        }
                    }
                }
            }
        }

        cout << "Skipping" << endl;
    }

    return mp4::track();
}
