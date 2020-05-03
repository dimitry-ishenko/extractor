#include "args.hpp"
#include <stdexcept>

#define DECLARE(except, msg) \
struct except : std::invalid_argument \
{ \
    explicit except(const std::string& arg) : \
        std::invalid_argument(std::string()+msg+": "+arg) \
    { } \
}

DECLARE(duplicate_option, "Duplicate option"    );
DECLARE(invalid_option  , "Invalid option"      );
DECLARE(extra_param     , "Extraneous parameter");
DECLARE(missing_param   , "Missing parameter"   );

std::string usage(const std::string& name)
{
    return "Usage: " + name + R"( [option] <audio>

Where <audio> is the path to an audio file with embedded transcript stored in
the metadata track with MIME audio/transcription_1. Such files are created,
for example, by the Google Recorder app on Android.

Option is one or more of the following:

    --help              Print this help screen and exit.

    --rate[=<n>]        Add time-stamps to the transcript before every <n>-th
                        paragraph. If <n> is omitted, it is assumed to be 1.

    --save-to=<script>  Save the transcript to <script> instead of the default
                        location.)";
}

auto starts_with(const std::string& arg, const std::string& opt)
{
    return arg.compare(0, opt.size(), opt) == 0;
}

args read_args(int argc, char* argv[])
{
    struct args args;

    for(int n = 1; n < argc; ++n)
    {
        std::string arg = argv[n];
        if(arg.empty()) continue;

        if(arg == "-h" || arg == "--help")
        {
            args.help = true;
            return args;
        }
        else if(starts_with(arg, "--save-to="))
        {
            if(args.script.empty())
            {
                args.script = arg.substr(arg.find('=') + 1);
            }
            else throw duplicate_option(arg);
        }
        else if(arg == "--time")
        {
            if(!args.rate) args.rate = 1;
            else throw duplicate_option(arg);
        }
        else if(starts_with(arg, "--time="))
        {
            if(!args.rate)
            {
                auto rate = arg.substr(arg.find('=') + 1);
                args.rate = std::stoi(rate);

                if(args.rate < 1) throw invalid_option(arg);
            }
            else throw duplicate_option(arg);
        }
        else if(arg[0] == '-')
        {
            throw invalid_option(arg);
        }
        else
        {
            if(args.audio.empty()) args.audio = arg;
            else throw extra_param(arg);
        }
    }

    if(args.audio.empty()) throw missing_param("<audio>");
    if(args.script.empty()) args.script = args.audio + ".txt";

    return args;
}
