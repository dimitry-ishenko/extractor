#include "args.hpp"
#include "mp4_file.hpp"
#include "mp4_track.hpp"
#include "tran_script.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[])
{
    auto name = std::filesystem::path(argv[0]).filename().string();

    int code = 0;
    try
    {
        auto args = read_args(argc, argv);
        if(args.help)
        {
            std::cout << usage(name) << std::endl;
        }
        else if(args.version)
        {
            std::cout << name << " version " << PROJECT_VERSION << std::endl;
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
