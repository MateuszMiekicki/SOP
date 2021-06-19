#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <stdio.h>
#include <streambuf>
#include <string.h>
#include <string>
#include <sys/signalfd.h>
#include <thread>
#include <unistd.h>

std::string readContentFromFile(const char *fileName)
{
    auto file = std::ifstream(fileName);
    return {(std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()};
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    {
        std::cerr << "sigprocmask\n";
        return -1;
    }
    auto signalFileDescriptor = signalfd(-1, &mask, 0);
    if (signalFileDescriptor < 0)
    {
        std::cerr << "signalfd";
        return -1;
    }
    std::mutex textMutex;
    std::string text = readContentFromFile(argv[1]);
    std::atomic<bool> status(true);
    auto changeContent = [&textMutex, &signalFileDescriptor, &text, &argv, &status]()
    {
        while (true)
        {
            struct signalfd_siginfo si;
            auto res = read(signalFileDescriptor, &si, sizeof(si));
            if (res < 0)
            {
                std::cerr << "read";
                status = false;
                return;
            }
            if (res != sizeof(si))
            {
                std::cerr << "Something wrong\n";
                status = false;
                return;
            }

            if (si.ssi_signo == SIGHUP)
            {
                std::lock_guard<std::mutex> lock(textMutex);
                text = readContentFromFile(argv[1]);
            }
            else if (si.ssi_signo == SIGINT or si.ssi_signo == SIGTERM)
            {
                status = false;
                return;
            }
        }
    };
    std::thread th(changeContent);
    th.detach();
    while (status)
    {
        {
            std::lock_guard<std::mutex> lock(textMutex);
            std::cout << text << std::endl;
        }
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    close(signalFileDescriptor);
}