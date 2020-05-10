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

constexpr auto npos = std::string::npos;

std::string usage(const std::string& name)
{
    return "Usage: " + name + R"( [option] <audio>

Where <audio> is the path to an audio file with embedded transcript stored in
the metadata track with MIME audio/transcription_1. Such files are created,
for example, by the Google Recorder app on Android.

Option is one or more of the following:

    --help              Print this help screen and exit.

    --save-to=<script>  Save the transcript to a location specified by
                        <script>. The <script> may contain special tokens:
                        %p - path of the audio file, %n - name of the audio
                        file and %e - extension. If this option is omitted,
                        the transcript is saved in "%p/%n.%e.txt".

    --time[=<n>]        Add time-stamps to the transcript before every <n>-th
                        paragraph. If <n> is omitted, it is assumed to be 1.)";
}

auto starts_with(const std::string& arg, const std::string& opt)
{
    return arg.compare(0, opt.size(), opt) == 0;
}

args read_args(int argc, char* argv[])
{
    struct args args;
    std::string script;

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
            if(script.empty())
            {
                script = arg.substr(arg.find('=') + 1);
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
    if(script.empty()) script = "%p/%n.%e.txt";

    std::string path, name = args.audio, ext;

    auto p = name.rfind('/');
    if(p != npos)
    {
        path = name.substr(0, p);
        name.erase(0, p + 1);
    }

    p = name.rfind('.');
    if(p != npos)
    {
        ext = name.substr(p + 1);
        name.erase(p);
    }

    bool pc = false;
    for(auto ch : script)
    {
        if(pc)
        {
            switch(ch)
            {
            case '%': args.script += '%'; break;
            case 'p': args.script += path; break;
            case 'n': args.script += name; break;
            case 'e': args.script += ext ; break;
            default : args.script += '%'; args.script += ch;
            }
            pc = false;
        }
        else if(ch == '%') pc = true;
        else args.script += ch;
    }
    if(pc) args.script += '%';

    return args;
}
