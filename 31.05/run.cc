#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

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
            arguments_.reserve(args.size() + 1);
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
        void parent([[maybe_unused]] int sig)
        {
            std::cerr << "Detected SIGINT.\n";
        }

        void child([[maybe_unused]] int sig)
        {
            std::cerr << "Caught signal in CHILD.\n";
        }

        class Signal
        {
            using funcptr = void (*)(int);
            inline static pid_t pid_;
            static void catchSignal_(int signo)
            {
                switch (signo)
                {
                case SIGTERM:
                {
                    kill(pid_, SIGTERM);
                }
                break;
                case SIGQUIT:
                {
                    kill(pid_, SIGQUIT);
                }
                break;
                case SIGHUP:
                {
                    kill(pid_, SIGHUP);
                }
                break;
                default:
                {
                    std::cerr << "can't catch signal\n";
                }
                break;
                }
            }

        public:
            Signal(const pid_t pid)
            {
                Signal::pid_ = pid;
            }
            operator funcptr() const
            {
                return reinterpret_cast<funcptr>(&Signal::catchSignal_);
            }
        };

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
        std::signal(SIGINT, &signalHandler::child);
        std::signal(SIGINT, SIG_IGN);
        exec.exec();
        return 0;
    }
    break;
    default:
    {
        std::signal(SIGINT, &signalHandler::parent);
        auto sig = signalHandler::Signal(pid);
        std::signal(SIGTERM, sig);
        std::signal(SIGQUIT, sig);
        std::signal(SIGHUP, sig);
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