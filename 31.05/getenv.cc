#include <algorithm>
#include <iostream>
#include <stdio.h>
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
    auto pathChosenByUser = std::getenv(args.getArguments().at(0).c_str());
    if (!pathChosenByUser)
    {
        std::cerr << "this path does not exist\n";
    }
    std::cout << pathChosenByUser;
}