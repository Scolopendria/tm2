#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"
#include <vector>

megaString scheduler(megaString tm2);
node schedule(node account);
metaContainer wheel(metaContainer day);
std::vector<metaContainer> collide(
    std::vector<metaContainer> sheduleBook,
    metaContainer item,
    std::size_t i,
    int ceiling,
    int floor
);
metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal);
attributeContainer decontainerize(metaContainer day);

megaString scheduler(megaString tm2){
    for (auto&& child : tm2.child.getChildren()){
        tm2.child.forge(schedule(child));
    }
    return tm2;
}

node schedule(node account){
    for (int i{}; i < 14; i++){
        account.attributes = decontainerize(
            wheel(
                pruneIrrelevantTasks(
                    metaContainer{account.get("Goals")},
                    calTime{i}
                ).extract()
            )
        );
    }

    return account;
}

metaContainer wheel(metaContainer day){// dysfunctional
/*
    while(!day.children.empty()){
        for (auto /*fix*//*child : day.children){// require '&'?
            bool scheduled{false};
            int autoStart{std::stoi(day.attributes.get("start"))};// MINVH
            for (auto scheduledTask : day.children){
                if (autoStart < scheduledTask.getTask().getStart()){
                    day.children = collide(
                        day.children,
                        child,
                        1,
                        day.getTask().getStart(),
                        day.getTask().getEnd()
                    );
                }
            }
            if (!scheduled){
                day.children = collide(
                    day.children,
                    child,
                    1,
                    day.getTask().getStart(),
                    day.getTask().getEnd()
                );
            }
        }
    }
*/
   return day;
}

std::vector<metaContainer> collide(
    std::vector<metaContainer> sheduleBook,
    metaContainer item,
    std::size_t i,
    int ceiling,
    int floor
){
/*
    //initializations
    int ceilValue{startCeiling}, floorValue{1440},
        totalUsedTime{std::stoi(item.attributes.get("time")) + 1}, allocatedTime, nextHead;
    std::size_t iter{}, ceiling, floor, slider;
    // set iters
    while (iter != scheduleBook.size()){
        if (start >= scheduleBook[iter].getEnd()) iter++; //trying to schedule before
        else break;
    }
    floor = iter;
    ceiling = iter;
    if (iter == scheduleBook.size()) nextHead = 1440;
    else nextHead = scheduleBook[ceiling].getStart();
    // check to see if nextHead is greater than start
    // if true, that means scheduling is in open space
    // and ceiling is set to the item above it
    if (nextHead > start) ceiling--;
    //find top boulder
    while (ceiling != std::string::npos){
        if (
            nav(scheduleBook[ceiling].getName(), gRoot)->get("strict") == "NULL" &&
            ceilValue < scheduleBook[ceiling].getStart()
        ) ceiling--;
        else break;
    }
    if (ceiling != std::string::npos)
        ceilValue = std::max(ceilValue, scheduleBook[ceiling].getEnd());

    //find bottom boulder
    while (floor != scheduleBook.size()){
        if (nav(scheduleBook[floor].getName(), gRoot)->get("strict") == "NULL") floor++;
        else break;
    }
    if (floor != scheduleBook.size()) floorValue = scheduleBook[floor].getStart();
    //calculate allocatedTime and totalUsedTime
    allocatedTime = floorValue - ceilValue;
    for (slider = ceiling + 1; slider < floor; slider++){
        totalUsedTime += 1 + scheduleBook[slider].getTimeUsed();
    }
    // if time is not enough, find LP element and chuck it
    // if lowest element is boulder, reschedule disregarding the boulder
    // chuck not implemented 
    if (allocatedTime > totalUsedTime){
        for (slider = ceiling + 1; slider < iter; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTimeUsed()),
                scheduleBook[slider].getName()
            );
            ceilValue += 1 + scheduleBook[slider].getTimeUsed();
        }

        sPath->attribute(
            stdTimeRep(ceilValue) + "-" + stdTimeRep(ceilValue + std::stoi(item.attributes.get("time"))),
            item.fullpath
        );
        ceilValue += 1 + std::stoi(item.attributes.get("time"));

        for (slider = iter; slider < floor; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTimeUsed()),
                scheduleBook[slider].getName()
            );
            ceilValue += 1 + scheduleBook[slider].getTimeUsed();
        }
         return true;
    }
    //recalculate, if item is lowest prioirty return false
    return false;
*/
    return sheduleBook;
}

metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal){
    // simplified version
    for (std::size_t i{completeList.children.size()-1}; i != SIZE_MAX; i--){
        if (completeList.children[i].attributes.get("date") != cal.strDate){
            completeList.children.erase(completeList.children.begin() + i);
        } else {
            pruneIrrelevantTasks(completeList.children[i], cal);
        }
    }

    return completeList;
}

attributeContainer decontainerize(metaContainer day){
    attributeContainer scheduleBook{};

    if (day.getName() == "self") scheduleBook.set(day.getTask().getFullStdTime(), day.getTask().getName());// redundant

    for (auto &&child : day.children){
        scheduleBook.set(decontainerize(child).getList());
    }

    return scheduleBook;
}

#endif