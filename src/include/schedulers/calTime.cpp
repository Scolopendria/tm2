#ifndef CALTIME_CPP
#define CALTIME_CPP

#include "schedulers.h"
#include <chrono>

class calTime{
    public:
        calTime();
        void init(int offset);
        int year{};
        int month{};
        int date{};
        int weekday{};
        int minute_t{};
        std::string strDate{};
        std::string baseDate{};
};

calTime::calTime(){
    std::stringstream ss{};
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    )};
    auto ltm{std::localtime(&now)};
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    date = ltm->tm_mday;
    ss << year << ":" << month << ":" << date;
    baseDate = ss.str();
};

void calTime::init(int offset){
    std::stringstream ss{};
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now() + std::chrono::days(offset)
    )};
    auto ltm{std::localtime(&now)};
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    date = ltm->tm_mday;
    weekday = ltm->tm_wday;
    minute_t = (ltm->tm_hour * 60) + ltm->tm_min;
    ss << year << ":" << month << ":" << date;
    strDate = ss.str();
    if (offset) minute_t = 0;
};

#endif