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
        auto at(const std::size_t index) const
        {
            return arguments_.at(index);
        }
    };
}

int main(int argc, char **argv)
{
    if (auto args = ArgsParser(argc, argv); args)
    {
        if (auto pathChosenByUser = std::getenv(args.at(0).c_str());
            pathChosenByUser)
        {
            std::cout << pathChosenByUser;
        }
        else
        {
            std::cerr << "this path does not exist\n";
        }
    }
}