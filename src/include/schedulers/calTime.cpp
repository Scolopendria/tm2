#ifndef CALTIME_CPP
#define CALTIME_CPP

#include "schedulers.h"
#include <chrono>

// class calTime{
//     public:
//         calTime();
//         calTime(int offset);
//         calTime* initialize(int offset);
//         int year{};
//         int month{};
//         int date{};
//         int weekday{};
//         int minute_t{};
//         std::string strDate{};
//         std::string baseDate{};
// };

// calTime::calTime(){
//     std::stringstream ss{};
//     auto now{std::chrono::system_clock::to_time_t(
//         std::chrono::system_clock::now()
//     )};
//     auto ltm{std::localtime(&now)};
//     this->year = 1900 + ltm->tm_year;
//     this->month = 1 + ltm->tm_mon;
//     this->date = ltm->tm_mday;
//     ss << year << ":" << month << ":" << date;
//     this->baseDate = ss.str();
// };


// calTime::calTime(int offset){
//     *this = calTime{};
//     this->initialize(offset);
// }

// calTime* calTime::initialize(int offset){
//     std::stringstream ss{};
//     auto now{std::chrono::system_clock::to_time_t(
//         std::chrono::system_clock::now() + std::chrono::days(offset)
//     )};
//     auto ltm{std::localtime(&now)};
//     this->year = 1900 + ltm->tm_year;
//     this->month = 1 + ltm->tm_mon;
//     this->date = ltm->tm_mday;
//     this->weekday = ltm->tm_wday;
//     this->minute_t = (ltm->tm_hour * 60) + ltm->tm_min;
//     ss << year << ":" << month << ":" << date;
//     this->strDate = ss.str();
//     this->minute_t *= (!offset);
//     return this;
// };

#endif