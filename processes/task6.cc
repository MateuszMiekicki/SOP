#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void signalHandler(int signal)
{
    auto pid = getpid();
    auto strSignal = strsignal(signal);
    switch (signal)
    {
    case SIGALRM:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGHUP:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGINT:
    {
        printf("process %d got signal %s\n", pid, strSignal);
        exit(SIGINT);
    }
    break;
    case SIGPIPE:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGQUIT:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGTERM:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGUSR1:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    case SIGUSR2:
    {
        printf("process %d got signal %s\n", pid, strSignal);
    }
    break;
    default:
    {
    }
    }
}

int main()
{
    struct sigaction psa;
    memset(&psa, 0, sizeof(psa));
    psa.sa_handler = signalHandler;
    sigaction(SIGINT, &psa, nullptr);
    sigaction(SIGALRM, &psa, nullptr);
    sigaction(SIGHUP, &psa, nullptr);
    sigaction(SIGINT, &psa, nullptr);
    sigaction(SIGPIPE, &psa, nullptr);
    sigaction(SIGQUIT, &psa, nullptr);
    sigaction(SIGTERM, &psa, nullptr);
    sigaction(SIGUSR1, &psa, nullptr);
    sigaction(SIGUSR2, &psa, nullptr);
}