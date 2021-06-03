#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

class ArgsParser
{
private:
    std::string programName_;
    std::vector<std::string> arguments_;

public:
    ArgsParser(int argc, char **argv) : programName_{argv[0]},
                                        arguments_(argv + 1, argv + argc)
    {
    }
    operator bool() const
    {
        return arguments_.size();
    }
    auto getProgramName() const
    {
        return programName_;
    }
    auto getArguments() const
    {
        return arguments_;
    }
};

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
    if ('-' != args.getArguments().at(0).at(0))
    {
        if (setenv(args.getArguments().at(0).c_str(),
                   args.getArguments().at(1).c_str(),
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
        auto pathToDelete = args.getArguments().at(0);
        std::cout << std::string(pathToDelete.begin() + 1, pathToDelete.end()).c_str();
        unsetenv(std::string(pathToDelete.begin() + 1, pathToDelete.end()).c_str());
        execl(shell, shell, nullptr);
        std::cerr << "Error starting shell\n";
        return -2;
    }
}
