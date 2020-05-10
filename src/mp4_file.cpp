#include "mp4_file.hpp"

#include <iostream>
#include <stdexcept>

namespace mp4
{

constexpr auto meta = AP4_ATOM_TYPE('m', 'e', 't', 'a');
constexpr auto mett = AP4_ATOM_TYPE('m', 'e', 't', 't');

constexpr auto npos = std::string::npos;

file::file(const std::filesystem::path& path)
{
    AP4_ByteStream* s;

    std::cout << "Opening audio file: " << path << std::endl;
    auto res = AP4_FileByteStream::Create(path.string().data(), AP4_FileByteStream::STREAM_MODE_READ, s);
    if(AP4_FAILED(res)) throw std::invalid_argument("Failed to open file");

    stream_.reset(s);
    file_.reset(new AP4_File { *s });
}

track file::find_trans()
{
    auto& tracks = file_->GetMovie()->GetTracks();
    auto count = tracks.ItemCount();
    std::cout << "Found: " << count << " tracks" << std::endl;

    for(decltype(count) n = 0; n < count; ++n)
    {
        AP4_Track* trk;

        std::cout << "Processing track: " << n << std::endl;
        auto res = tracks.Get(n, trk);
        if(AP4_FAILED(res)) throw std::invalid_argument("Failed to access track");

        if(trk->GetHandlerType() == meta)
        {
            std::cout << "Detected: meta track" << std::endl;

            auto table = trk->GetSampleTable();
            if(!table) throw std::invalid_argument("Missing sample table");

            auto count = table->GetSampleDescriptionCount();
            std::cout << "Found: " << count << " sample descriptions" << std::endl;

            for(decltype(count) n = 0; n < count; ++n)
            {
                std::cout << "Processing sample description: " << n << std::endl;
                auto desc = table->GetSampleDescription(n);

                if(desc->GetFormat() == mett)
                {
                    std::cout << "Detected: mett format" << std::endl;
                    if(auto atom = dynamic_cast<AP4_UnknownSampleEntry*>(desc->ToAtom()))
                    {
                        auto payload = atom->GetPayload();
                        std::string type { payload.GetData(), payload.GetData() + payload.GetDataSize() };

                        auto pos = type.find('\0');
                        if(pos != npos) type.resize(pos);

                        // Bento4 eats the first 8 bytes of "application/transcription_1"
                        // thinking they are Reserved and Data reference index fields (6+2)
                        // as per the QuickTime spec

                        if(type == "ion/transcription_1")
                        {
                            std::cout << "Detected: application/transcription_1 type" << std::endl;
                            return track(trk);
                        }
                    }
                }
            }
        }

        std::cout << "Skipping" << std::endl;
    }

    return track();
}

}
