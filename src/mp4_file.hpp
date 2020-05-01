#ifndef MP4_FILE_HPP
#define MP4_FILE_HPP

#include "mp4_track.hpp"
#include "Ap4.h"

#include <memory>
#include <string>

namespace mp4
{

class file
{
public:
    explicit file(const std::string&);
    track find_trans();

private:
    std::unique_ptr<AP4_ByteStream> stream_;
    std::unique_ptr<AP4_File> file_;
};

}

#endif
