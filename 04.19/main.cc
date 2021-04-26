#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

enum class TimeOfDay
{
    MORNING,
    DAY,
    EVENING,
    NIGHT
};

class Time
{
private:
    unsigned hour_;
    unsigned minute_;
    unsigned second_;

public:
    Time() = default;
    Time(unsigned h, unsigned m, unsigned s) : hour_{h},
                                               minute_{m},
                                               second_{s}
    {
    }

    std::string to_string() const
    {
        std::ostringstream ss;
        ss << std::setw(2)
           << std::setfill('0')
           << std::to_string(hour_) << ':'
           << std::to_string(minute_) << ':'
           << std::to_string(second_);
        return ss.str();
    }

    TimeOfDay timeOfDay() const
    {
        if (hour_ < 3)
            return TimeOfDay::NIGHT;
        if (hour_ < 6)
            return TimeOfDay::MORNING;
        if (hour_ < 18)
            return TimeOfDay::DAY;
        if (hour_ < 22)
            return TimeOfDay::EVENING;
        return TimeOfDay::NIGHT;
    }

    void nextSecond()
    {
        second_++;
        if (60 <= second_)
        {
            second_ = 0;
            nextMinute();
        }
    }

    void nextMinute()
    {
        minute_++;
        if (60 <= minute_)
        {
            minute_ = 0;
            nextHour();
        }
    }

    void nextHour()
    {
        hour_++;
        if (24 <= hour_)
        {
            hour_ = 0;
        }
    }
};

int main()
{
    auto time = Time{23, 58, 59};
    std::cout << time.to_string() << '\n';

    time.nextMinute();
    std::cout << time.to_string() << '\n';

    time.nextSecond();
    std::cout << time.to_string() << '\n';

    time.nextHour();
    std::cout << time.to_string() << '\n';

    switch (time.timeOfDay())
    {
    case TimeOfDay::MORNING:
    {
        std::cout << "rano\n";
    }
    break;
    case TimeOfDay::DAY:
    {
        std::cout << "dzien\n";
    }
    break;
    case TimeOfDay::EVENING:
    {
        std::cout << "wieczor\n";
    }
    break;
    case TimeOfDay::NIGHT:
    {
        std::cout << "noc\n";
    }
    break;
    }
}
