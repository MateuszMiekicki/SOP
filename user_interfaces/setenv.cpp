#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
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
    auto args = ArgsParser(argc, argv);
    if (!args)
    {
        return 0;
    }
    auto shell = std::getenv("SHELL");
    if (!shell)
    {
        std::cerr << "Not found SHELL env. var.\n";
        return -1;
    }
    if ('-' != args.at(0).front())
    {
        if (setenv(args.at(0).c_str(),
                   args.at(1).c_str(),
                   1))
        {
            std::cerr << "Error setting env. var.\n";
            return -1;
        }
        execl(shell, shell, nullptr);
        std::cerr << "Error starting shell\n";
        return -2;
    }
    else
    {
        auto [begin, end] = std::make_pair(args.at(0).begin() + 1,
                                           args.at(0).end());
        unsetenv(std::string(begin, end).c_str());
        execl(shell, shell, nullptr);
        std::cerr << "Error starting shell\n";
        return -2;
    }
}