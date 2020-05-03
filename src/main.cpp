#include "args.hpp"
#include "mp4_file.hpp"
#include "mp4_track.hpp"
#include "tran_script.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[])
{
    int code = 0;
    std::string name = argv[0];

    try
    {
        auto args = read_args(argc, argv);
        if(args.help)
        {
            std::cout << usage(name) << std::endl;
        }
        else
        {
            mp4::file file { args.audio };
            tran::script script;

            auto track = file.find_trans();

            auto data = track.read_sample();
            while(data.size())
            {
                script.add_words(data);
                data = track.read_sample();
            }

            script.save_to(args.script, args.rate);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        code = 1;
    }

    return code;
}
