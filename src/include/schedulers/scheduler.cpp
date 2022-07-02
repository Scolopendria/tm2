#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"
#include <vector>
#include <iostream>
#include <random>

megaString scheduler(megaString tm2);
node schedule(node account);
metaContainer wheel(metaContainer day);
metaContainer collide(metaContainer day);
metaContainer demote(metaContainer day);
metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime tempor);
attributeContainer decontainerize(metaContainer day);

megaString scheduler(megaString tm2){
    std::srand(std::time(nullptr));
    std::vector<node> children{tm2.child.getChildren()};// unnecessary buffer
    for (auto &&child : children){
        tm2.child.forge(schedule(child));
    }

    return tm2;
}

node schedule(node account){
    for (int i{}; i < 3; i++){
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
                                *metaContainer{account.get("Goals").getChildren(), tempor}.extract(),
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
    // param niche of this function, wheel, and underling function, collide, is the same
    // they do have different functional roles, though
    // schedule children
    while (!day.children.empty()){
        day = collide(day);
        day = demote(day);
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
        const bool bounded{day.children[i].attributes.get("bounded") == "true"};
        const int start{
            [bounded](int start){
                if (!bounded) return -1;
                return start;
            }(superstoi(day.children[i].attributes.get("start"), -1))
        };
        std::vector<int> boundedList;
        for (std::size_t j{}; j < day.scheduledChildren.size(); j++){
            if (day.scheduledChildren[j].attributes.get("bounded") == "true"){
                boundedList.push_back(j);// need iterator
            }
        }

        if (totalTimeUsed > day.getTask().getEnd() - day.getTask().getStart()) continue;

        if (boundedList.empty()){
            if (bounded){
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

            continue;
        }

        if (bounded){
            if (start < day.scheduledChildren[boundedList.front()].getTask().getStart()){
                if (start + totalTimeUsed < day.scheduledChildren[boundedList.front()].getTask().getStart()){
                    day.initialize(i, start);
                }
                continue;
            }

            if (start > day.scheduledChildren[boundedList.back()].getTask().getEnd()){
                if (start + totalTimeUsed < day.getTask().getEnd()){
                    day.initialize(i, start);
                }
                continue;
            }
        } else {
            int availableTime{day.scheduledChildren[boundedList.front()].getTask().getStart() - day.getTask().getStart()-1};
            for (std::size_t j{}; j < boundedList.front(); j++){
                availableTime -= day.scheduledChildren[j].getTotalTime();
            }

            if (totalTimeUsed < availableTime){
                day.initialize(i, day.getTask().getStart());
                continue;
            }

            availableTime = day.getTask().getEnd() - day.scheduledChildren[boundedList.back()].getTask().getEnd()-1;
            for (std::size_t j{boundedList.back()+(std::size_t)1}; j < day.scheduledChildren.size(); j++){
                availableTime -= day.scheduledChildren[j].getTotalTime();
            }

            if (totalTimeUsed < availableTime){
                day.initialize(i, day.scheduledChildren[boundedList.back()].getTask().getEnd()+1);
                continue;
            }
        }

        if (boundedList.size() != 1){
            for (std::size_t j{}; j < boundedList.size()-2; j++){
                if (bounded){
                    if (start < day.scheduledChildren[boundedList[j+1]].getTask().getStart()){
                        if (start + totalTimeUsed < day.scheduledChildren[boundedList[j+1]].getTask().getStart()){
                            day.initialize(i, start);
                        }
                        break;
                    }
                    continue;
                }

                int availableTime{// lowerbound - higherbound
                    day.scheduledChildren[boundedList[j+1]].getTask().getStart() -
                    day.scheduledChildren[boundedList[j]].getTask().getEnd()
                };

                for (std::size_t k{boundedList[j]+(std::size_t)1}; k < boundedList[j+1]; k++){
                    availableTime -= day.scheduledChildren[k].getTotalTime();
                }

                if (totalTimeUsed < availableTime){
                    day.initialize(i, day.scheduledChildren[boundedList[j]].getTask().getEnd()+1);
                    break;
                }
            }
        }
    }

    return day;
}

metaContainer demote(metaContainer day){
    for (std::size_t i{day.children.size()-1}; i != SIZE_MAX; i--){
        day.children[i].attributes.set(
            "priority",
            std::to_string((int)(std::stof(day.children[i].attributes.get("priority")) * (float)0.78))
        );                       // MINVH but error checks shoiuld be handled during construction

        if (std::stoi(day.children[i].attributes.get("priority")) < (std::rand() % 60 + 20)){
            day.children.erase(day.children.begin() + i);
        }
    }

    return day;
}

metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime tempor){
    // simplified version
    // no repeating day support
    // shellCast behaviour uncharted
    for (std::size_t i{completeList.children.size()-1}; i != SIZE_MAX; i--){
        if (completeList.children[i].attributes.get("date") == tempor.strDate)
            completeList.children[i] = pruneIrrelevantTasks(completeList.children[i], tempor);
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