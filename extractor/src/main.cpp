#include "mp4_file.hpp"
#include "mp4_track.hpp"
#include "trans.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    int code = 0;
    std::string app = argv[0];

    try
    {
        if(argc != 3) throw std::invalid_argument(
            "Usage: " + app + " <audio-file-path> <transcript-path>"
        );

        mp4::file file { argv[1] };
        trans::script script;

        auto track = file.find_trans();

        auto sample = track.read_sample();
        while(sample.size())
        {
            script.add_words(trans::decode(sample));
            sample = track.read_sample();
        }

        script.save_to(argv[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
        code = 1;
    }

    return code;
}
