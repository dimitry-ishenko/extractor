#ifndef MP4_FILE_HPP
#define MP4_FILE_HPP

#include "mp4_track.hpp"
#include "Ap4.h"

#include <filesystem>
#include <memory>

namespace mp4
{

class file
{
public:
    explicit file(const std::filesystem::path&);
    track find_trans();

private:
    std::unique_ptr<AP4_ByteStream> stream_;
    std::unique_ptr<AP4_File> file_;
};

}

#endif
