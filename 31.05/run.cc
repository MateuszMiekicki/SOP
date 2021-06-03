#include <algorithm>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
    enum
    {
        ERROR = -1,
        CHILD_PROCESS = 0
    };
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

    class Exec
    {
    private:
        std::string programName_;
        std::vector<char *> arguments_;
        void convert_(std::vector<std::string> args)
        {
            arguments_.reserve(1 + args.size() + 1);
            std::transform(args.begin(), args.end(),
                           std::back_inserter(arguments_),
                           [](std::string &s)
                           {
                               return s.data();
                           });
            arguments_.push_back(nullptr);
        }

    public:
        Exec(const std::string &programName,
             const std::vector<std::string> &args) : programName_{programName}
        {
            convert_(args);
        }
        void exec() const
        {
            //Note: mkssoftware.com/docs/man3/execl.3.asp differences between exc*
            execvp(programName_.data(), const_cast<char **>(arguments_.data()));
        }
    };
    namespace signalHandler
    {
        void parent(int sig)
        {
            std::cerr << "They got back together!\n";
        }

        void child(int sig)
        {
            std::cerr << "Caught signal in CHILD.\n";
        }

    }
}

int main(int argc, char **argv)
{

    auto args = ArgsParser(argc, argv);
    if (!args)
    {
        return 0;
    }

    switch (auto pid = fork(); pid)
    {
    case ERROR:
    {
        std::cerr << "call to fork failed\n";
        return -1;
    }
    break;
    case CHILD_PROCESS:
    {
        auto exec = Exec(args.getArguments().at(0), args.getArguments());
        signal(SIGINT, &signalHandler::child);
        exec.exec();
        return 0;
    }
    break;
    default:
    {
        signal(SIGINT, &signalHandler::parent);
        if (int state; waitpid(pid, &state, 0) > 0)
        {
            if (WIFEXITED(state) && !WEXITSTATUS(state))
            {
                std::cout << "program is executed successfully\n";
                return 0;
            }
            else if (WIFSIGNALED(state))
            {
                std::cerr << "the child process terminated from receiving a "
                             "signal that wasn't caught\n";
                return -1;
            }
            else if (WIFEXITED(state) && WEXITSTATUS(state))
            {
                if (WEXITSTATUS(state) == 127)
                {
                    std::cerr << "Execution failed\n";
                    return -1;
                }
                else
                {
                    std::cerr << "program terminated with non-zero status\n";
                    return -1;
                }
            }
            else
            {
                std::cerr << "program didn't terminate normally\n";
                return -1;
            }
        }
    }
    break;
    }
}