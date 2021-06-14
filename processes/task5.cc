#include <chrono>
#include <iostream>
#include <random>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

unsigned randomNaturalNumber(const unsigned max)
{
    const unsigned min{1};
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<> randomNumber(min, max);
    return randomNumber(gen);
}

int returnRandomValue()
{
    return randomNaturalNumber(10) % 2;
}

int main()
{
    enum
    {
        ERROR = -1,
        CHILD_PROCESS = 0
    };
    const unsigned MAX_SLEEP_TIME{5};
    bool firstChildFlag = true;
    while (firstChildFlag)
    {
        switch (auto firstChild = fork(); firstChild)
        {
        case ERROR:
        {
            return -1;
        }
        break;
        case CHILD_PROCESS:
        {
            std::this_thread::sleep_for(std::chrono::seconds(randomNaturalNumber(MAX_SLEEP_TIME)));
            bool secondChildFlag = true;
            while (secondChildFlag)
            {
                switch (auto secondChild = fork(); secondChild)
                {
                case ERROR:
                {
                    return -1;
                }
                break;
                case CHILD_PROCESS:
                {
                    std::this_thread::sleep_for(std::chrono::seconds(randomNaturalNumber(MAX_SLEEP_TIME)));
                    bool thirdChildFlag = true;
                    while (thirdChildFlag)
                    {
                        switch (auto thirdChild = fork(); thirdChild)
                        {
                        case ERROR:
                        {
                            return -1;
                        }
                        break;
                        case CHILD_PROCESS:
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(randomNaturalNumber(MAX_SLEEP_TIME)));
                            bool fourthChildFlag = true;
                            while (fourthChildFlag)
                            {
                                switch (auto fourthChild = fork(); fourthChild)
                                {
                                case ERROR:
                                {
                                    return -1;
                                }
                                break;
                                case CHILD_PROCESS:
                                {
                                    std::this_thread::sleep_for(std::chrono::seconds(randomNaturalNumber(MAX_SLEEP_TIME)));
                                    return returnRandomValue();
                                }
                                break;
                                default:
                                {
                                    int wstatus;
                                    waitpid(-1, &wstatus, 0);
                                    fourthChildFlag = WEXITSTATUS(wstatus);
                                }
                                break;
                                }
                            }
                            return returnRandomValue();
                        }
                        break;
                        default:
                        {
                            int wstatus;
                            waitpid(-1, &wstatus, 0);
                            thirdChildFlag = WEXITSTATUS(wstatus);
                        }
                        break;
                        }
                    }
                    return returnRandomValue();
                }
                break;
                default:
                {
                    int wstatus;
                    waitpid(-1, &wstatus, 0);
                    secondChildFlag = WEXITSTATUS(wstatus);
                }
                break;
                }
            }
            return returnRandomValue();
        }
        break;
        default:
        {
            int wstatus;
            waitpid(-1, &wstatus, 0);
            firstChildFlag = WEXITSTATUS(wstatus);
        }
        break;
        }
    }
}