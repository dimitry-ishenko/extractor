#include "mp4_file.hpp"
#include "mp4_track.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    int code = 0;
    std::string app = argv[0];
    cout << "Starting " << app << endl;

    try
    {
        if(argc != 2) throw std::invalid_argument("Usage: " + app + " <filename>");

        std::string name = argv[1];
        mp4::file file { name };

        auto track = file.find_trans();

        auto sample = track.read_sample();
        while(sample.is_valid())
        {
            // TODO
            sample = track.read_sample();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
        code = 1;
    }

    cout << "Exiting " << app << endl;
    return code;
}
