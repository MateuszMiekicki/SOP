#include <algorithm>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace
{
    class ArgsParser
    {
    private:
        std::vector<std::string> arguments_;

    public:
        ArgsParser(int argc, char **argv) : arguments_(argv + 1, argv + argc)
        {
        }
        operator bool() const
        {
            return arguments_.size();
        }
        auto getArguments() const
        {
            return arguments_;
        }
    };
}

int main(int argc, char **argv)
{

    if (auto args = ArgsParser(argc, argv); args)
    {
        auto pathChosenByUser = std::getenv(args.getArguments().at(0).c_str());
        if (!pathChosenByUser)
        {
            std::cerr << "this path does not exist\n";
        }
        std::cout << pathChosenByUser;
    }
}