#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void signalHandler(int signal, siginfo_t *siginfo, void *ucontext)
{
    printf("Received value %d\n", siginfo->si_value.sival_int);
}

int main()
{
    struct sigaction psa;
    psa.sa_sigaction = &signalHandler;
    psa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &psa, nullptr) == -1)
    {
        perror("sigusr: sigaction");
        return 0;
    }
    while (1)
    {
        sleep(2);
    }
}