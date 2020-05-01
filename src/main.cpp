#include "mp4_file.hpp"
#include "mp4_track.hpp"
#include "tran_script.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[])
{
    int code = 0;
    std::string app = argv[0];

    try
    {
        if(argc != 3) throw std::invalid_argument(
            "Usage: " + app + " <audio-file> <transcript>"
        );

        mp4::file file { argv[1] };
        tran::script script;

        auto track = file.find_trans();

        auto data = track.read_sample();
        while(data.size())
        {
            script.add_words(data);
            data = track.read_sample();
        }

        script.save_to(argv[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        code = 1;
    }

    return code;
}
