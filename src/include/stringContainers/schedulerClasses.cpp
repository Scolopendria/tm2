#ifndef SCHEDULERCLASSES_CPP
#define SCHEDULERCLASSES_CPP

#include "stringContainers.h"

task::task(std::string taskName, int start, int end){
    std::stringstream ss;

    this->name = taskName;
    this->startTime = start;
    this->endTime = end;
    this->timeUsed = end - start;

    ss << startTime / 60 << ":" << startTime % 60 << "-" << endTime / 60 << ":" << endTime % 60;
    this->stdTime = ss.str();
}

std::string task::getName(){
    return this->name;
}

std::string task::getFullStdTime(){
    return this->stdTime;
}

int task::getStart(){
    return this->startTime;
}

int task::getEnd(){
    return this->endTime;
}

int task::getTimeUsed(){
    return this->timeUsed;
}

collideResult::collideResult(bool scheduled, std::vector<metaContainer> scheduleBook){
    this->scheduled = scheduled;
    this->scheduleBook = scheduleBook;
}

#endif