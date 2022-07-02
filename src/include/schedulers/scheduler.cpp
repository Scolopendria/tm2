#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"
#include <vector>
#include <iostream>

megaString scheduler(megaString tm2);
node schedule(node account);
metaContainer wheel(metaContainer day);
metaContainer collide(metaContainer day);
metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime tempor);
attributeContainer decontainerize(metaContainer day);

megaString scheduler(megaString tm2){
    std::vector<node> children{tm2.child.getChildren()};
    for (auto &&child : children){
        tm2.child.forge(schedule(child));
    }

    return tm2;
}

node schedule(node account){
    for (int i{}; i < 3; i++){// assignment failing
        calTime tempor{i};
        account.forge(
            *account.get("Schedule").forge(
                [tempor](node account){
                    node temp{"\"" + tempor.strDate + "\"{}"};
                    temp.attributes = decontainerize(
                        wheel(
                            pruneIrrelevantTasks(
                                // switch to decide which task gets scheduled on which day before sending it to wheel
                                // basically prune irrevlevant task before wheel for better long term management
                                // introduce 'demotions' in this metapruner and cross-day info passing
                                *metaContainer{account.get("Goals").getChildren(), tempor.minute_t}.extract(),
                                tempor
                            )
                        )
                    );

                    std::cout << "pass: " << tempor.strDate << std::endl;

                    return temp;
                }(account)
            )
        );
    }

    return account;
}

metaContainer wheel(metaContainer day){
    // param niche of this function, wheel, and unnderling function, collide, is the same
    // they do have different functional roles, though
    // schedule children
    while (!day.children.empty()){
        day = collide(day);
        //day = demote(day);
    }
    // recursively schedule (spin through) scheduled-children
    std::vector<metaContainer> scheduledChildrenTree{};
    for (auto &&child : day.scheduledChildren){
        scheduledChildrenTree.push_back(wheel(child));
    }
    day.scheduledChildren = scheduledChildrenTree;

    return day;
}

metaContainer collide(metaContainer day){
    for (std::size_t i{day.children.size()-1}; i != SIZE_MAX; i--){
        const int totalTimeUsed{day.children[i].updateTotalTime()};
        const int start{superstoi(day.children[i].attributes.get("start"), 0)};
        std::vector<int> boundedChildrenList;
        for (std::size_t j{}; j < day.scheduledChildren.size(); j++){
            if (day.scheduledChildren[j].attributes.get("start") != "NULL"){
                boundedChildrenList.push_back(j);
            }
        }

        if (totalTimeUsed > day.getTask().getEnd() - day.getTask().getStart()) continue;

        if (boundedChildrenList.empty()){
            if (start){
                day.initialize(i, start);
                continue;
            }

            int availableTime{day.getTask().getEnd() - day.getTask().getStart()};
            for (auto &&child : day.scheduledChildren){
                availableTime -= child.getTotalTime();
            }

            if (totalTimeUsed < availableTime){
                day.initialize(i, day.getTask().getStart());
                continue;
            }
        }

        if (start){
            if (start < day.scheduledChildren[boundedChildrenList.front()].getTask().getStart()){
                if (start + totalTimeUsed < day.scheduledChildren[boundedChildrenList.front()].getTask().getStart()){
                    day.initialize(i, start);
                }
                continue;
            }

            if (start > day.scheduledChildren[boundedChildrenList.back()].getTask().getEnd()){
                if (start + totalTimeUsed < day.getTask().getEnd()){
                    day.initialize(i, start);
                }
                continue;
            }
        } else {
            int availableTime{day.scheduledChildren[boundedChildrenList.front()].getTask().getStart() - day.getTask().getStart()-1};
            for (std::size_t j{}; j < boundedChildrenList.front(); j++){
                availableTime -= day.scheduledChildren[j].getTotalTime();
            }

            if (totalTimeUsed < availableTime){
                day.initialize(i, day.getTask().getStart());
                continue;
            }

            availableTime = day.getTask().getEnd() - day.scheduledChildren[boundedChildrenList.back()].getTask().getEnd()-1;
            for (std::size_t j{boundedChildrenList.back()+(std::size_t)1}; j < day.scheduledChildren.size(); j++){
                availableTime -= day.scheduledChildren[j].getTotalTime();
            }

            if (totalTimeUsed < availableTime){
                day.initialize(i, day.scheduledChildren[boundedChildrenList.back()].getTask().getEnd()+1);
                continue;
            }
        }

        if (boundedChildrenList.size() != 1){
            for (std::size_t j{}; j < boundedChildrenList.size()-2; j++){
                if (start){
                    if (start < day.scheduledChildren[boundedChildrenList[j+1]].getTask().getStart()){
                        if (start + totalTimeUsed < day.scheduledChildren[boundedChildrenList[j+1]].getTask().getStart()){
                            day.initialize(i, start);
                        }
                        break;
                    }
                    continue;
                }

                int availableTime{// lowerbound - higherbound
                    day.scheduledChildren[boundedChildrenList[j+1]].getTask().getStart() -
                    day.scheduledChildren[boundedChildrenList[j]].getTask().getEnd()
                };

                for (std::size_t k{boundedChildrenList[j]+(std::size_t)1}; k < boundedChildrenList[j+1]; k++){
                    availableTime -= day.scheduledChildren[k].getTotalTime();
                }

                if (totalTimeUsed < availableTime){
                    day.initialize(i, day.scheduledChildren[boundedChildrenList[j]].getTask().getEnd()+1);
                    break;
                }
            }
        }
    }

    return day;
}

metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime tempor){
    // simplified version
    // no repeating day support
    // shellCast does not work properly
    for (std::size_t i{completeList.children.size()-1}; i != SIZE_MAX; i--){
        if (
            (completeList.children[i].attributes.get("date") == "NULL" && tempor.strDate == tempor.baseDate) ||
            completeList.children[i].attributes.get("date") == tempor.strDate
        ) completeList.children[i] = pruneIrrelevantTasks(completeList.children[i], tempor);
        else completeList.children.erase(completeList.children.begin() + i);
    }

    return completeList;
}

attributeContainer decontainerize(metaContainer day){
    attributeContainer scheduleBook{};

    if (day.getName() == "self") scheduleBook.set(day.getTask().getFullStdTime(), day.getTask().getName());

    for (auto &&child : day.scheduledChildren){
        scheduleBook.set(decontainerize(child).getList());
    }

    return scheduleBook;
}

#endif